#pragma once

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

#include <utility>

namespace cursed{

template< typename DIMENSION, typename VALUE_TYPE = int > 
struct Dimension{
    using axis = DIMENSION;
    using value_type = VALUE_TYPE;

    explicit Dimension( const value_type& val ) : _value(val) {}
    Dimension( const Dimension& ) = default;
    Dimension( ) : _value{} {}

    operator value_type() const{
        return _value;
    }

    operator value_type&() {
        return _value;
    }

    Dimension operator+( const Dimension& rhs ) const{
        return Dimension{_value + rhs._value};
    }

    Dimension operator-( const Dimension& rhs ) const{
        return Dimension{_value - rhs._value};
    }

    Dimension& operator=( const Dimension& rhs ) {
        _value = rhs._value;
        return *this;
    }

    Dimension& operator+=( const Dimension& rhs ){
        _value += rhs._value;
        return *this;
    }

    Dimension& operator-=( const Dimension& rhs ){
        _value -= rhs._value;
        return *this;
    }

    Dimension& operator=( const value_type& val ) {
        _value = val;
        return *this;
    }

    bool operator==( const value_type& val ) const {
        return _value == val;
    }

    bool operator==( const Dimension& rhs ) const {
        return _value == rhs._value;
    }

    bool operator>( const Dimension& rhs ) const{
        return _value > rhs._value;
    }

    bool operator>=( const Dimension& rhs ) const{
        return _value >= rhs._value;
    }

    bool operator<( const Dimension& rhs ) const{
        return _value < rhs._value;
    }

    bool operator<=( const Dimension& rhs ) const{
        return _value <= rhs.value;
    }

    value_type& valueRef(){ return _value; }
    value_type value() const{ return _value; }

private:
    value_type _value;
};

}