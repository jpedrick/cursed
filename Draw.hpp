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

#include "IWindow.hpp"
#include "Point.hpp"
#include "Direction.hpp"
#include "Log.hpp"
#include "Rectangle.hpp"
#include "BoxStyle.hpp"
#include <vector>

namespace cursed{

using ccstring = std::vector<cchar_t>;

class Draw{
public:
    Point getCursorPosition( IWindow* win );
    static void filledBlock( IWindow* win, const Rectangle& dimensions, wchar_t c = ' ' );
    static void box( IWindow* win, Rectangle, BoxStyle style = BoxStyle::RoundedCorners );
    static void line( IWindow* win, Direction d, int pos, wchar_t c );
    static void line( IWindow* win, Direction d, Point start, int len, wchar_t c );
    static void moveCursor( IWindow* win, Point position );
    static void character( IWindow* win, Point position, char data );
    static void wcharacter( IWindow* win, Point position, wchar_t data );
    static void text( IWindow* win, Point start, const char* data, size_t len );
    static void textLine( IWindow* win, Point start, const ccstring& );
    static void text( IWindow* win, Point start, const std::string& data );
    static void text( IWindow* win, Point start, const std::wstring& data );
    static void printf( IWindow* win, Point start, const char* fmt, ... ) GCC_PRINTFLIKE(3,4);
    static std::vector<cchar_t> convert( const std::wstring& wstr, const attr_t attrs, short colorPair );

    static void drawBorder( IWindow* win, BoxStyle boxStyle );

    struct AttributeGuard{
        AttributeGuard( IWindow* window, unsigned long flags );
        ~AttributeGuard( );
    private:
        IWindow* _window;
        attr_t _oldAttrs;
        short _oldColorPair;
    };
};

}
