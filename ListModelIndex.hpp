//  Copyright notice:
//  This file is part of Cursed.
//  Cursed is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  Cursed is also covered by GNU Affero General Public License with the 
//  following permission under the GNU Affero GPL version 3 section 7:
//      If you modify this Program, or any covered work, by linking or
//      combining it with other code, such other code is not for that reason
//      alone subject to any of the requirements of the GNU Affero GPL
//      version 3.
//
//  Cursed is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with Cursed.  If not, see <https://www.gnu.org/licenses/>.
//
//  You should also have received a copy of the GNU Affero General Public License
//  along with Cursed.  If not, see <https://www.gnu.org/licenses/>.
#pragma once
#include <string>

namespace cursed{

class IListModel;

class ListModelIndex{
public:
    ListModelIndex() : _model(nullptr), _rowIndex(-1), _valid(false)
    {}

    ListModelIndex( IListModel* model, int64_t rowIndex ) : 
        _model(model), 
        _rowIndex(rowIndex),
        _valid(true)
    {}
    const std::string& text( int role ) const;
    int attributes( int role ) const;

    int64_t rowIndex() const{ return _rowIndex; }

    IListModel* model(){ return _model; }
    const IListModel* model() const { return _model; }

    void updateIndexFromInsert( int64_t row, int64_t count ){
        if( _rowIndex >= row ) _rowIndex += count;
    }
    enum class RowDeleted : bool{
        True = true,
        False = false
    };
    RowDeleted updateIndexFromDelete( int64_t row, int64_t count ){
        if( _rowIndex >= row ){
            if( _rowIndex < row + count ){
                _valid = false;
                return RowDeleted::True;
            }
            _rowIndex -= count; // shift down index by count.
        }

        return RowDeleted::False;
    }
    bool valid() const { return _valid; }
private:
    IListModel* _model;
    int64_t _rowIndex;
    bool _valid;
};

inline bool operator==( const ListModelIndex& left, const ListModelIndex& right ){
    return ( left.valid() && right.valid() )  //< invalid index always fail
        && ( left.rowIndex() == right.rowIndex() ) 
        && ( left.model() == right.model() );
}

inline bool operator!=( const ListModelIndex& left, const ListModelIndex& right ){
    return !( left == right );
}

}

