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

#include "Log.hpp"
#include "Direction.hpp"


namespace cursed{

struct Point{
    int x;
    int y;
    int z;

    Point( int _x, int _y, int _z = 0 ) : x(_x), y(_y), z(_z) {}
    Point() : x(0), y(0), z(0) {}

    int& getPosition( Direction d ){
        if( d == Direction::Horizontal ) return x;
        if( d == Direction::Vertical ) return y;

        return z;
    }
};

inline Point operator+( const Point& p1, const Point& p2 ){
    return { p1.x + p2.x, p1.y + p2.y, p1.z + p2.z };
}

inline Point operator-( const Point& p1, const Point& p2 ){
    return { p1.x - p2.x, p1.y - p2.y, p1.z - p2.z };
}

inline bool operator==( const Point& p1, const Point& p2 ){
    return ( p1.x == p2.x ) && ( p1.y == p2.y ) && ( p1.z == p2.z );
}

inline bool operator!=( const Point& p1, const Point& p2 ){
    return !( p1 == p2 );
}

Log& operator<<( Log& o, const Point& r );

}

