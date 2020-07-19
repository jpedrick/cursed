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

template< typename AXIS, typename VALUE_TYPE = int > 
struct Dimension{
    using axis = AXIS;
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

template< typename AXIS>
inline Dimension<AXIS> operator*( int scale, Dimension<AXIS> d ){
    return { d * scale };
}

template< typename... DIMENSIONS >
struct MultiDimensionalValue: 
    private DIMENSIONS...
{
    MultiDimensionalValue( DIMENSIONS&&... dimensions ) : DIMENSIONS{ std::forward<DIMENSIONS>(dimensions) } ... { }

    MultiDimensionalValue( const DIMENSIONS&... dimensions ) : DIMENSIONS{ dimensions } ... { }

    MultiDimensionalValue() : DIMENSIONS{ 0 }... { }

    template<typename DIMENSION> DIMENSION& get() { return *this; }
    template<typename DIMENSION> const DIMENSION& get() const{ return *this; }

    template<typename DIMENSION> bool operator>(  const DIMENSION& dim ) const { return get<DIMENSION>() > dim; }
    template<typename DIMENSION> bool operator>=(  const DIMENSION& dim ) const { return get<DIMENSION>() >= dim; }

    template<typename DIMENSION> bool operator<(  const DIMENSION& dim ) const { return get<DIMENSION>() < dim; }
    template<typename DIMENSION> bool operator<=(  const DIMENSION& dim ) const { return get<DIMENSION>() <= dim; }

    bool operator==( const MultiDimensionalValue& rhs ){
        return ( (static_cast<const DIMENSIONS&>(*this) == static_cast<const DIMENSIONS&>(rhs)) && ... );
    }

    MultiDimensionalValue operator-( const MultiDimensionalValue& rhs ) const
    {
        return MultiDimensionalValue{ DIMENSIONS{static_cast<const DIMENSIONS&>( *this ) - static_cast<const DIMENSIONS&>( rhs )}... };
    }

    MultiDimensionalValue operator+( const MultiDimensionalValue& rhs ) const
    {
        return MultiDimensionalValue{ DIMENSIONS{static_cast<const DIMENSIONS&>( *this ) - static_cast<const DIMENSIONS&>( rhs )}... };
    }

    MultiDimensionalValue& operator+=( const MultiDimensionalValue& rhs ) {
        ( ( get<DIMENSIONS>() += rhs.get<DIMENSIONS>() ), ... );
        return *this;
    }

    MultiDimensionalValue& operator-=( const MultiDimensionalValue& rhs ) {
        ( ( get<DIMENSIONS>() += rhs.get<DIMENSIONS>() ), ... );
        return *this;
    }
};

template< typename... ARGS >
MultiDimensionalValue< ARGS... > operator+( const MultiDimensionalValue<ARGS...>& lhs, const MultiDimensionalValue<ARGS...>& rhs )
{
    return { { static_cast<const Dimension<ARGS>&>( lhs ) + static_cast<const Dimension<ARGS>&>( rhs ) }... };
}
}
