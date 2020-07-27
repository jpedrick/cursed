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
#include "MouseEventWindow.hpp"

#include "Direction.hpp"
#include "Signal.hpp"
#include "IListModel.hpp"
#include <unordered_map>
#include <functional>

namespace cursed{

class IListModel;
class ListModelIndex;
class IListItemDelegate;

struct ListViewIndex{
    int column;
    int row;

    ListViewIndex( int _column, int _row ) : column(_column), row(_row) {}
    ListViewIndex() : column(0), row(0) {}

    int getPosition( Direction d ) const {
        if( d == Direction::Horizontal ) return column;
        if( d == Direction::Vertical ) return row;
        return -1;
    }
    void setPosition( Direction d, int v ) {
        if( d == Direction::Horizontal ) column = v;
        if( d == Direction::Vertical ) row = v;
    }

    inline bool operator==( const ListViewIndex& other ) const{
        return other.row == row && other.column == column;
    }
};

template<typename T> struct IndexHash;
template<>
struct IndexHash<ListViewIndex>{
    size_t operator()(const ListViewIndex& ix ) const{
        return (size_t)ix.row << 32 | (size_t)ix.column;
    }
};

class ListView : public MouseEventWindow{
public:
    template< typename... Args >
    ListView( IListModel* dataModel,  Args&&... args ) : 
        MouseEventWindow( std::forward<Args>(args)... ),
        _viewport(-1,-1)
    { 
        delegates.clear();
        setDataModel( dataModel );
    }

    typedef Signal< const ListModelIndex& signal_field(index), int signal_field(role) > CellEvent;

    struct : Window {
        CellEvent cellClicked;
    } signals;

    IListModel* model(){ return _model; }

    void setDataModel( IListModel* );
    void reset();

    void adjustViewportTopLeft( int row, int column );
    void setViewportTopLeft( int row, int column );

    void setDefaultDelegate( IListItemDelegate* delegate );
    void setRoleDelegate( int column, IListItemDelegate* delegate );
    void setRowDelegate( int row, IListItemDelegate* delegate );
    void setCellDelegate( ListViewIndex ix, IListItemDelegate* delegate );

    const IListItemDelegate* defaultDelegate( ) const;
    const IListItemDelegate* roleDelegate( int role ) const;
    const IListItemDelegate* rowDelegate( int row ) const;
    const IListItemDelegate* cellDelegate( ListViewIndex ix ) const;

    void draw( bool fullRefresh );
    int visibleRows() const;
    void setRoleStripingColors( unsigned long evenColorPair, unsigned long oddColorPair );

protected:
    void onMouseInput( const Point& relative, MouseButtonEvent& e ) override;

    void onDataChanged( const ListModelIndex&, const ListModelIndex& );
    void onRowsInserted( int64_t begin, int64_t count );
    void onRowsRemoved( int64_t begin, int64_t count );

    IListModel* _model;
    struct{
        IConnectionToken::pointer  dataChanged;
        IConnectionToken::pointer rowsInserted;
        IConnectionToken::pointer  rowsRemoved;
    } connections;

    Point _viewport;
    struct RoleGeometry{
        int startX;
        int width;

        bool contains( int x ) const{
            return x >= startX && x < startX + width;
        }
    };
    std::unordered_map<int,RoleGeometry> _roleGeometry;
    int64_t firstVisibleRow() const;
    Rectangle indexGeometry( const ListModelIndex&, int role ) const;
    ListView::RoleGeometry roleGeometry( int role ) const;
    void draw( const ListModelIndex& ix, int role, IListItemDelegate* delegate );

    std::vector<int> _displayRoles;

    struct{
        std::unordered_map<int, IListItemDelegate* > role;
        std::unordered_map<int, IListItemDelegate* > row;
        std::unordered_map< ListViewIndex, IListItemDelegate*, IndexHash<ListViewIndex> > cell;
        IListItemDelegate* _default = nullptr;
        void clear(){
            role.clear();
            row.clear();
            cell.clear();
            _default = nullptr;
        }
    } delegates;

    struct{
        unsigned long evenRoleColors;
        unsigned long oddRoleColors;
    } visualProperties;
};

}
