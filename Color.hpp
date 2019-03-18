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
#include <type_traits>

namespace cursed{

    class ColorPool;

    enum class ColorTarget{
        Forground,
        Background
    };

    struct Color{ 
        Color( int r, int g, int b );
        Color( const Color& );
        ~Color();

        int id() const { return _id; }
    private:
        const int r; 
        const int g; 
        const int b; 
        short _id;
    };

    class ColorPair{
    public:
        ColorPair( const Color fg, const Color bg );
        ~ColorPair();
        const Color& fg() const{ return _fg; }
        const Color& bg() const{ return _bg; }

        const Color& get( ColorTarget tgt ) const{
            switch( tgt ){
            case ColorTarget::Forground:
                return _fg;
            case ColorTarget::Background:
                return _bg;
            }
        }

        operator unsigned long( ) const;
    private:
        Color _fg;
        Color _bg;
        short _id;
    };

}
