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
#include "MultiDimensionalValue.hpp"

namespace cursed{

struct XAxis;
struct YAxis;
struct ZAxis;

using X = Dimension<XAxis,int>;
using Y = Dimension<YAxis,int>;
using Z = Dimension<ZAxis,int>;

class Point : public MultiDimensionalValue<X,Y,Z> {
public:
    using MultiDimensionalValue<X,Y,Z>::MultiDimensionalValue;
    Point( int x = 0, int y = 0, int z = 0 ) : MultiDimensionalValue { X{x}, Y{y}, Z{z} } {}
    Point( const MultiDimensionalValue<X,Y,Z>& v ) : MultiDimensionalValue { v } {}

    int& getPosition( Direction d ){
        if( d == Direction::Horizontal ) return get<X>().valueRef();
        if( d == Direction::Horizontal ) return get<Y>().valueRef();

        return get<Z>().valueRef();
    }

    int getPosition( Direction d ) const{
        if( d == Direction::Horizontal ) return get<X>().value();
        if( d == Direction::Horizontal ) return get<Y>().value();

        return get<Z>().value();
    }

    X& x() { return this->get<X>(); }
    Y& y() { return this->get<Y>(); }
    Z& z() { return this->get<Z>(); }

    const X& x() const { return this->get<X>(); }
    const Y& y() const { return this->get<Y>(); }
    const Z& z() const { return this->get<Z>(); }
};

inline Point xyz( const X& x = X{}, const Y& y = Y{}, const Z& z = Z{} ){
    return Point( x, y, z );
}

inline Point xyz( int x = 0, int y = 0, int z = 0 ){
    return Point( X{x}, Y{y}, Z{z} );
}

inline int& getPosition( Point& p, Direction d ){
    if( d == Direction::Horizontal ) return p.x();
    if( d == Direction::Horizontal ) return p.y();

    return p.get<Z>();
}

inline int getPosition( const Point& p, Direction d ){
    if( d == Direction::Horizontal ) return p.x();
    if( d == Direction::Horizontal ) return p.y();

    return p.z();
}

Log& operator<<( Log& o, const Point& r );

}

