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
#include "Signal.hpp"

#include <cstdint>
#include <string>
#include <vector>

namespace cursed{
class ListModelIndex;
class IListModel{
public:

    typedef Signal< const ListModelIndex& signal_field(first), const ListModelIndex& signal_field(last) > DataChanged;
    typedef Signal< int64_t signal_field(begin), int64_t signal_field(count) > RowsInserted;
    typedef Signal< int64_t signal_field(begin), int64_t signal_field(count) > RowsRemoved;
    struct {
        DataChanged  dataChanged;
        RowsInserted rowsInserted;
        RowsRemoved  rowsRemoved;
    } signals;

    virtual ~IListModel(){}
    virtual int64_t rowCount() const = 0;
    virtual std::vector<int> displayRoles() const = 0;
    virtual const std::string& roleName( int ) const = 0;

    virtual const std::string& text( const ListModelIndex& ix, int displayRole ) const = 0;
    virtual unsigned long attributes(  const ListModelIndex& ix, int displayRole ) const = 0;
    virtual ListModelIndex index( int row ) const;
};

}
