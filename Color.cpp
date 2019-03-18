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
#include "Color.hpp"
#include <ncursesw/curses.h>
#include <stdexcept>
#include <iostream>
#include <vector>
#include <array>

namespace cursed{

template< typename T >
std::vector<T> range( T start, T end, const std::vector<T>& c = {} ){
    if( start == end ) return c;
    std::vector<T> r = c;
    r.push_back(start);
    return range<T>( start + 1, end, r );
}

static std::vector<short> _freeColorIDs = range<short>( COLOR_WHITE + 1, 256 );
static std::vector<short> _freeColorPairIDs = range<short>( COLOR_WHITE + 1, 256 );

static std::array<short, 256> colorIDRefcounts;
static std::array<short, 256> colorPairIDRefcounts;

Color::Color( int r, int g, int b ) : r(r), g(g), b(b) {
    if( _freeColorIDs.empty() ) throw std::runtime_error("exceeded maximum number of colors");

    _id = _freeColorIDs.back(); 
    _freeColorIDs.pop_back();

    colorIDRefcounts[_id] = 1;

    init_color( _id, r, g, b );
}

Color::Color( const Color& c ) : r(c.r), g(c.g), b(c.b), _id(c._id) {
}

Color::~Color(){
    --colorIDRefcounts[_id];

    if( colorIDRefcounts[_id] == 0 ) _freeColorIDs.push_back(_id);
}
        
ColorPair::ColorPair( const Color fg, const Color bg ) : _fg(fg), _bg(bg), _id( 0 ) {
    if( _freeColorPairIDs.empty() ) throw std::runtime_error("exceeded maximum number of color pairs");

    _id = _freeColorPairIDs.back(); 
    _freeColorPairIDs.pop_back();

    colorPairIDRefcounts[_id] = 1;

    init_pair(_id,_bg.id(), _fg.id());
}

ColorPair::~ColorPair(){
    --colorPairIDRefcounts[_id];

    if( colorPairIDRefcounts[_id] == 0 ) _freeColorIDs.push_back(_id);
}

ColorPair:: operator unsigned long( ) const{
    return COLOR_PAIR(_id);
}

}
