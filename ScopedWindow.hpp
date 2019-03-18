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

extern "C"{
#include <ncursesw/curses.h>
}

namespace cursed{
class ScopedWindow{
public:
    ScopedWindow() {}
    ScopedWindow( WINDOW* w ) : _window(w){ }

    ~ScopedWindow(){ reset(); }

    void reset( WINDOW* w = nullptr ){ if( _window ) delwin(_window); _window = w; }

    WINDOW* get(){ return _window; }

    operator bool() const { return _window; }
private:
    WINDOW* _window = nullptr;
};

}
