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
#include <locale>
#include <codecvt>
#include <string>
#include <memory>
#include <cassert>
#include <sstream>
#include <vector>
#include <algorithm>

namespace cursed{

namespace boxCharacters{
    struct Corners{
        const char* upperLeft; 
        const char* upperRight;
        const char* bottomRight;
        const char* bottomLeft;
    };
    struct Edges{
        const char* horizontal; 
        const char* vertival;
        const char* horizontalTLeft; 
        const char* horizontalTRight; 
        const char* verticalTTop; 
        const char* verticalTBottom; 
        const char* cross;  
        const char* leftVertical;  
        const char* rightVertical; 
    };                             

    // TODO: figure out how to draw diagonal boxes or use these chars
    // Diagonal box chars:  🮢🮡 🮠🮣 🮤 🮥 🮦 🮧 🮨 🮪 🮫 🮬 🮭 🮮 🮯
    
    constexpr Corners diagonalCorners = { "🮣", "🮢", "🮠", "🮡" };
    constexpr Corners roundedCorners = { "╭", "╮", "╯", "╰" };
    constexpr Corners sharpCorners   = { "┌", "┐", "┘", "└" };

    constexpr Corners fancyCorners1   = { sharpCorners.upperLeft, roundedCorners.upperRight, sharpCorners.bottomRight, roundedCorners.bottomLeft };
    constexpr Corners fancyCorners2   = { roundedCorners.upperLeft, sharpCorners.upperRight, roundedCorners.bottomRight, sharpCorners.bottomLeft };

    constexpr Edges lightEdges = { "─", "│", "├", "┤", "┬", "┴", "┼", "⎸", "⎹" };

    using WCharConverter = std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t>;
    const WCharConverter converter;
}

namespace boxStyle{
    struct BoxCharacters{
        boxCharacters::Corners corners;
        boxCharacters::Edges edges;
    };

    //   🮣──────────────🮢
    //   │ Diagonal Box │
    //   🮡──────────────🮠
    constexpr BoxCharacters Diagonal = BoxCharacters{
        boxCharacters::diagonalCorners,
        boxCharacters::lightEdges
    };

    //   ┌─────────────╮
    //   │ Fancy Box 1 │
    //   ╰─────────────┘
    constexpr BoxCharacters Fancy1 = BoxCharacters{
        boxCharacters::fancyCorners1,
        boxCharacters::lightEdges
    };

    //   ╭─────────────┐
    //   │ Fancy Box 2 │
    //   └─────────────╯
    constexpr BoxCharacters Fancy2 = BoxCharacters{
        boxCharacters::fancyCorners2,
        boxCharacters::lightEdges
    };

    //   ╭─────────────🮢
    //   │ Fancy Box 3 │
    //   🮡─────────────╯

    //   🮣─────────────╮
    //   │ Fancy Box 4 │
    //   ╰─────────────🮠
    
    //   ┌───────────┐
    //   │ Sharp Box │
    //   └───────────┘
    constexpr BoxCharacters Sharp = BoxCharacters{
        boxCharacters::sharpCorners,
        boxCharacters::lightEdges
    };

    //   ╭─────────────╮
    //   │ Rounded Box │
    //   ╰─────────────╯
    constexpr BoxCharacters Rounded = BoxCharacters{
        boxCharacters::roundedCorners,
        boxCharacters::lightEdges
    };

    const BoxCharacters& lookup( BoxStyle style ){
        switch( style ){
            case BoxStyle::RoundedCorners: return Rounded;
            case BoxStyle::SharpCorners: return Sharp;
            case BoxStyle::Fancy1: return Fancy1;
            case BoxStyle::Fancy2: return Fancy2;
            case BoxStyle::Diagonal: return Diagonal;
            default: throw std::runtime_error("unknown box style");
        }
    }

    wchar_t getWChar( const char* ch ){
      boxCharacters::WCharConverter converter;
      const auto wstr = converter.from_bytes( ch );

      assert( wstr.size() == 1 );

      return wstr[0];
    }
}


void Draw::drawBorder( IWindow* win, BoxStyle boxStyle ){
    if ( boxStyle != BoxStyle::None ){
        const auto& style = boxStyle::lookup( boxStyle );

        const auto vline = boxStyle::getWChar( style.edges.vertival );
        const auto hline = boxStyle::getWChar( style.edges.horizontal );
        const auto ul = boxStyle::getWChar( style.corners.upperLeft );
        const auto ur = boxStyle::getWChar( style.corners.upperRight );
        const auto lr = boxStyle::getWChar( style.corners.bottomRight );
        const auto ll = boxStyle::getWChar( style.corners.bottomLeft );

        wborder( win->window(), vline, vline, hline, hline, ul, ur, ll, lr );
    }
}

void Draw::box( IWindow* win, Rectangle dim, BoxStyle boxStyle ){
    Point topLeft{0,0};
    Point topRight = topLeft;
    topRight.x += dim.size.width - 1;

    Point bottomRight = topLeft;
    bottomRight.y += dim.size.height - 1;
    bottomRight.x += dim.size.width - 1;

    Point bottomLeft = topLeft;
    bottomLeft.y += dim.size.height - 1;

    if ( boxStyle != BoxStyle::None ){
        const auto& style = boxStyle::lookup( boxStyle );

        wchar_t vline = boxStyle::getWChar( style.edges.vertival );
        wchar_t hline = boxStyle::getWChar( style.edges.horizontal );

        cursed::Draw::line( win, cursed::Direction::Vertical, 0, ACS_VLINE );
        cursed::Draw::line( win, cursed::Direction::Vertical, dim.size.width - 1, ACS_VLINE );

        cursed::Draw::line( win, cursed::Direction::Horizontal, 0, ACS_HLINE );
        cursed::Draw::line( win, cursed::Direction::Horizontal, dim.size.height-1, ACS_HLINE );

        cursed::Draw::text( win, topLeft, style.corners.upperLeft, ::strlen(style.corners.upperLeft) ); // Upper left
        cursed::Draw::text( win, topRight, style.corners.upperRight, ::strlen(style.corners.upperRight) ); // Upper Right

        cursed::Draw::text( win, bottomRight, style.corners.bottomRight, ::strlen(style.corners.bottomRight) ); // Lower right
        cursed::Draw::text( win, bottomLeft, style.corners.bottomLeft, ::strlen(style.corners.bottomLeft ) ); // Lower Left 
    }
}

void Draw::line( IWindow* win, Direction d, int pos, wchar_t c ){
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

void Draw::line( IWindow* win, Direction d, Point start, int len, wchar_t c ){
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

void Draw::filledBlock( IWindow* win, const Rectangle& dimensions, wchar_t c ){
    Point lineStart = dimensions.topLeft;
    Direction dir = dimensions.size.height > dimensions.size.width ? Direction::Vertical : Direction::Horizontal;

    int lineLength = dimensions.size.getDimension( dir );
    for( int i = 0; i < dimensions.size.getDimension( other(dir) ); ++i ){
        line( win, dir, lineStart, lineLength, c );
        lineStart.getPosition( other(dir) ) += 1;
    }
}

std::vector<cchar_t> Draw::convert( const std::wstring& wstr, const attr_t attrs, short colorPair ){
    std::vector<cchar_t> ostr(wstr.size());
    std::transform( wstr.begin(), wstr.end(), ostr.begin(), [attrs,colorPair]( wchar_t wc ){
        cchar_t cc;
        ::setcchar( &cc, &wc, attrs, colorPair, nullptr );
        return cc;
    });

    return ostr;
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

        cchar_t cc;
        ::setcchar( &cc, &data, 0, 0, nullptr );
        
        mvwadd_wchstr( win->window(), p.y, p.x, &cc ); 
    }
}

void Draw::character( IWindow* win, Point position, char data ){ 
    Rectangle dim = win->dimensions();
    if( dim.containsRelative(position) ){
        Point p = win->absolute( position );
        mvwaddch( win->window(), p.y, p.x, data ); 
    }
}


void Draw::textLine( IWindow* win, Point start, const ccstring& str ){
    Point p = win->absolute( start );

    auto len = str.size();

    if( len == 0 ) return;

    Rectangle dim = win->dimensions();

    size_t requiredSize = str.size();

    int displayLen = std::min( dim.size.width - start.x, (int)requiredSize );
    displayLen = std::max( displayLen, 0 );

    if( dim.containsRelative(start) && displayLen > 0 ){
        mvwadd_wchnstr( win->window(), p.y, p.x, str.data(), str.size() );
    }
}

void Draw::text( IWindow* win, Point start, const char* data, size_t len ){
    Draw::text( win, start, std::string(data) );
}


void Draw::text( IWindow* win, Point start, const std::string& data ){ 
    boxCharacters::WCharConverter converter;

    text( win, start, converter.from_bytes( data ) );
}

void Draw::text( IWindow* win, Point start, const std::wstring& wstr ){
    std::wstringstream ss( wstr );

    std::wstring line;
    Point pos = start;
    while( std::getline( ss, line ) ){
        Draw::textLine( win, pos, convert(line, 0, 0 ) ); 
        pos.y += 1;
    }
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
