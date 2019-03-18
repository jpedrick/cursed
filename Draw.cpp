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
#include "Draw.hpp"

namespace cursed{

void Draw::box( IWindow* win, Rectangle dim ){
    Point topLeft = dim.topLeft;
    Point topRight = dim.topLeft;
    topRight.x += dim.size.width;

    Point bottomRight = dim.topLeft;
    topRight.y += dim.size.height;
    topRight.x += dim.size.width;

    Point bottomLeft= dim.topLeft;
    topRight.y += dim.size.height;

    cursed::Draw::line( win, cursed::Direction::Vertical, topLeft,    dim.size.height, ACS_VLINE );
    cursed::Draw::line( win, cursed::Direction::Vertical, topRight,   dim.size.height, ACS_VLINE );

    cursed::Draw::line( win, cursed::Direction::Vertical, topLeft,    dim.size.width, ACS_HLINE );
    cursed::Draw::line( win, cursed::Direction::Vertical, bottomLeft, dim.size.width, ACS_HLINE );

    cursed::Draw::character( win, topLeft,     ACS_ULCORNER ); // Upper left
    cursed::Draw::character( win, bottomRight, ACS_LRCORNER ); // Lower right

    cursed::Draw::character( win, bottomLeft,  ACS_LLCORNER ); // Lower left
    cursed::Draw::character( win, topRight,    ACS_URCORNER ); // Upper Right
}

void Draw::line( IWindow* win, Direction d, int pos, char c ){
    switch(d){
        case Direction::Horizontal:{
            if( pos < win->dimensions().size.height ){                           
                Point p = win->absolute( {0,pos} );
                mvwhline( win->window(), p.y, p.x, c, win->dimensions().size.width );
            }
        }break;
        case Direction::Vertical:{
            if( pos < win->dimensions().size.width ){                           
                Point p = win->absolute( {pos,0} );
                mvwvline( win->window(), p.y, p.x, c, win->dimensions().size.height );
            }
        }break;
        default: break;
    }
}

void Draw::line( IWindow* win, Direction d, Point start, int len, char c ){
    Point p = win->absolute( start );
    switch(d){
        case Direction::Horizontal:{
            if( p.y < win->dimensions().size.height ){                           
                int maxlen = std::min( len, win->dimensions().size.width - ( start.x + len ) );
                if( maxlen > 0 ){
                    mvwhline( win->window(), p.y, p.x, c, maxlen );
                }
            }
        }break;
        case Direction::Vertical:{
            if( p.x < win->dimensions().size.width ){                           
                int maxlen = std::min( len, win->dimensions().size.width - ( start.x + len ) );
                if( maxlen > 0 ){
                    mvwvline( win->window(), p.y, p.x, c, maxlen );
                }
            }
        }break;
        default: break;
    }
}

Draw::AttributeGuard::AttributeGuard( IWindow* window, unsigned long flags ) : _window(window) { 
    attr_get( &_oldAttrs, &_oldColorPair, nullptr );
    ::attron( flags );
}
Draw::AttributeGuard::~AttributeGuard( ){ attr_set( _oldAttrs, _oldColorPair, nullptr ); }

Point Draw::getCursorPosition( IWindow* win ){
    return { getcurx(win->window()), getcury(win->window()) };
}

void Draw::moveCursor( IWindow* win, Point position ){ 
    Point p = win->absolute( position );
    ::wmove( win->window(), p.y, p.x );
}

void Draw::wcharacter( IWindow* win, Point position, wchar_t data ){
    Rectangle dim = win->dimensions();
    if( dim.containsRelative(position) ){
        Point p = win->absolute( position );
        mvwaddch( win->window(), p.y, p.x, data ); 
    }
}

void Draw::character( IWindow* win, Point position, char data ){ 
    Rectangle dim = win->dimensions();
    if( dim.containsRelative(position) ){
        Point p = win->absolute( position );
        mvwaddch( win->window(), p.y, p.x, data ); 
    }
}

void Draw::text( IWindow* win, Point start, const char* data, size_t len ){
    Point p = win->absolute( start );

    len = strnlen( data, len );
    while( len > 0 && data[len - 1] == '\0' ) --len;

    if( len == 0 ) return;

    Rectangle dim = win->dimensions();

    int displayLen = std::min( dim.size.width - start.x, (int)len );
    displayLen = std::max( displayLen, 0 );

    if( dim.containsRelative(start) && displayLen > 0 ){
        mvwaddnstr( win->window(), p.y, p.x, data, displayLen );
    }
}

void Draw::text( IWindow* win, Point start, const std::string& data ){ 
    Draw::text( win, start, data.data(), data.length() ); 
}

void Draw::printf( IWindow* win, Point start, const char* fmt, ... ){
    char buf[2048];
    va_list args;
    va_start( args, fmt );
    int len = ::vsnprintf( buf, sizeof(buf), fmt, args );
    va_end(args);
    text( win, start, buf, len );
}

}
