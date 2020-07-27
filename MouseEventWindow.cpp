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
#include "MouseEventWindow.hpp"
#include <thread>

namespace cursed{

namespace{
    constexpr std::array<int,3> _buttons = { 1, 2, 3 };
}

MouseEventWindow::MouseEventWindow( Direction layout, const std::string& name, std::initializer_list<LayoutObject> children ) : 
    Window( layout, name, children ) 
{ 
}

void MouseEventWindow::onMouseInput( const Point& relative, MouseButtonEvent& e ){
    bool contained = dimensions().containsRelative( relative );
    bool moved = e.moveEvent();

    if( e._consumed ) {
        contained = false;
    }

    if( contained ){
        e._consumed = true;
    }

    if( moved && contained ){ 
        signals.mouseDrag.emit( _lastPosition, relative );
        _lastPosition = relative;
    }

    bool pressed[ _buttons.size() ] = { false };
    bool released[ _buttons.size() ] = { false };
    for ( auto button : _buttons ){
        bool clicked  = e.mouseKeyEvent( 1, MouseButtonEventType::Clicked );
        bool clicked2 = e.mouseKeyEvent( 1, MouseButtonEventType::DoubleClicked );
        bool clicked3 = e.mouseKeyEvent( 1, MouseButtonEventType::TripleClicked );

        pressed[button]  = e.mouseKeyEvent( button, MouseButtonEventType::Pressed );
        released[button] = _pressed[button] && !pressed[button] ? true : false;
        if( pressed[button] ){
            onMouseButtonPressed( button, contained, relative );
        }
        if( released[button] || !contained ){
            onMouseButtonReleased( button );
        }

        if( contained && clicked ){
            signals.clicked( button );
        }

        if( contained && clicked2 ){
            signals.doubleClicked( button );
        }

        if( contained && clicked3 ){
            signals.tripleClicked( button );
        }
    }

    if( contained && !_mouseContained ){
        _mouseContained = true;
        signals.mouseEnter();
        _lastPosition = relative;
    }

    if( !contained && _mouseContained ){
        _mouseContained = false;
        signals.mouseExit();
    }

    if( _mouseContained && _lastPosition != relative ){
        signals.mouseMove( relative, _lastPosition );
        _lastPosition = relative;
    }

    Window::onMouseInput( relative, e );
}

void MouseEventWindow::onMouseButtonPressed( int button, bool contained, const Point& relativePos ){
    if( contained ){
        if( !_pressed[button] ){
            _pressed[button] = true;
            signals.pressed( button );
            _pressedPoint[button] = relativePos;
        }
    }
    else {
        onMouseButtonReleased( button );
    }
}

void MouseEventWindow::onMouseButtonReleased( int button ){
    if( _pressed[button] ){
        _pressed[button] = false;
        signals.released( button );
    }
}

}
