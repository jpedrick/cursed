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
#include "Button.hpp"
#include "Draw.hpp"
#include <thread>

namespace cursed{

void Button::onMouseInput( const Point& relative, MouseButtonEvent& e ){
    bool contained = dimensions().containsRelative( relative );
    bool moved = e.moveEvent();

    if(  moved && contained ){ 
        signals.mouseDrag.emit( _pressedPoint, relative );
        return;
    }
    if( e._consumed ) contained = false;

    if( contained ) e._consumed = true;

    bool clicked  = e.mouseKeyEvent( 1, MouseButtonEventType::Clicked );
    bool clicked2 = e.mouseKeyEvent( 1, MouseButtonEventType::DoubleClicked );
    bool clicked3 = e.mouseKeyEvent( 1, MouseButtonEventType::TripleClicked );

    bool pressed  = e.mouseKeyEvent( 1, MouseButtonEventType::Pressed );
    bool released = _pressed && !pressed ? true : false;

    if( contained && pressed ){
        _pressed = true; draw(true); ::refresh();
        signals.pressed();
        _pressedPoint = relative;
    }

    if( contained && clicked ){
        signals.clicked();
        _pressed = true; draw(true); ::refresh();
        signals.released();
        addDelayedAction( std::chrono::milliseconds(200),[&]{
            _pressed = false; draw(true); ::refresh();
        }); 
    }

    if( contained && clicked2 ){
        signals.doubleClicked( );
        _pressed = true; draw(true); ::refresh();
        addDelayedAction( std::chrono::milliseconds(200),[&]{
            _pressed = false; draw(true); ::refresh();
        }); 
    }

    if( contained && clicked3 ){
        signals.tripleClicked( );
        _pressed = true; draw(true); ::refresh();
        addDelayedAction( std::chrono::milliseconds(200),[&]{
            _pressed = false; draw(true); ::refresh();
        }); 
    }

    if( _pressed && released ){
        _pressed = false; draw(true); ::refresh();
        signals.released( );
    }

    if( _pressed && !contained ){
        _pressed = false; draw(true); ::refresh();
    }

    Window::onMouseInput( relative, e );
}

void Button::draw( bool fullRefresh ){
    if( fullRefresh ){
        unsigned long color = _pressed ? visualProperties.pressedColor : visualProperties.normalColor;
        Draw::AttributeGuard ag{ this, color | visualProperties.attributes };

        Rectangle dim = dimensions();

        for( int y = 0; y < dim.size.height; ++y ){
            Draw::line( this, Direction::Horizontal, y, ' ');
        }

        int midw = dim.size.width / 2;
        int midy = dim.size.height / 2;

        int midl = _text.length();

        int startx = std::max(0, midw - midl );
        Point labelPosition{ startx, midy };

        Draw::text( this, {startx,midy}, _text );
    }
}

}
