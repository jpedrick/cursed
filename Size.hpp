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
#include "Direction.hpp"
#include "Forward.hpp"

namespace cursed{

struct Size{
    Size() = default;
    Size( int w, int h ) : width(w), height(h) {}
    int width = 0;
    int height = 0;

    int& getDimension( Direction d ){
        if( d == Direction::Horizontal ) {
            return width;
        }else{
            return height;
        }
    }

    int getDimension( Direction d ) const{
        if( d == Direction::Horizontal ) {
            return width;
        }else{
            return height;
        }
    }

    void adjust( Direction d, int amt ){
        if( d == Direction::Horizontal ) {
            width += amt;
        }else{
            height += amt;
        }
    }
};

Log& operator<<( Log& o, const Size& s );

}

