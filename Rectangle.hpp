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

#include "Forward.hpp"
#include "Point.hpp"
#include "Size.hpp"

namespace cursed{

struct Rectangle{
    Point topLeft;
    Size  size;

    inline bool contains( const Point& p ) const{
        int relx = p.x - topLeft.x;
        int rely = p.y - topLeft.y;

        return ( relx >= 0 && relx < size.width ) && ( rely >= 0 && rely < size.height );
    }
    inline bool containsRelative( const Point& p ) const{
        return ( p.x >= 0 && p.y >= 0 ) && ( p.x < size.width ) && ( p.y < size.height );
    }
};

Log& operator<<( Log& o, const Rectangle& r );

}
