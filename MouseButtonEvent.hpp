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

#include "Point.hpp"

extern "C"{
#include <ncursesw/curses.h>
}

namespace cursed{

//TODO: map inside mouseKeyEvent;
enum class MouseButtonEventType : int{
    Released      = NCURSES_BUTTON_RELEASED,
    Pressed       = NCURSES_BUTTON_PRESSED,
    Clicked       = NCURSES_BUTTON_CLICKED,
    DoubleClicked = NCURSES_DOUBLE_CLICKED,
    TripleClicked = NCURSES_TRIPLE_CLICKED
};

class MouseButtonEvent{
public:
    MouseButtonEvent(int k) : _key(k) { poll(); }
    void poll();
    bool valid() const { return _key == KEY_MOUSE; }

    Point position() const { return _position; }

    bool mouseKeyEvent( int button, MouseButtonEventType e ) const;
    bool moveEvent() const;

    mmask_t _buttonState;
    Point _position;
    int _key;
    bool _consumed = false;
};

inline Log& operator<<( Log& o, const MouseButtonEvent& event );

}
