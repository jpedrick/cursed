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

#include "LayoutObject.hpp"
#include "ScopedWindow.hpp"
#include "Direction.hpp"
#include "BoxLayout.hpp"
#include "Window.hpp"
#include "Reactor.hpp"

extern "C"{
#include <ncursesw/curses.h>
}
#include "ScopedWindow.hpp"

#include <chrono>

namespace cursed{

class Application : public Window {
public:
    Application( const std::string& name = "application", 
                 Direction dir = Direction::Horizontal, 
                 std::initializer_list<LayoutObject> children = {} );

    ~Application();

    void start();

    void onKeyboardInput( int c ) override;
    void onMouseInput( const Point& relative, MouseButtonEvent& e ) override;
    bool exit() const{ return _exit; }

    void onRootResized();
    void run( );

    Reactor& reactor(){ return _reactor; }
    static Application& instance();

    void refresh( bool force = true ) override;

    IWindow* focus() const{ return _focus; }
    void setFocus( IWindow* newFocus ) { 
        if( newFocus != _focus ){
            if( _focus ) _focus->signals.lostFocus.emit();
            if( newFocus ) newFocus->signals.acquiredFocus.emit();
        }
        _focus = newFocus;
    }

    Application* application() override{ return this; }

    void addDelayedAction( std::chrono::milliseconds delay, std::function<void()> action ) override;
private:
    void processDelayedActions();
    WINDOW* window() override { return _window.get(); }

    bool _exit = false;
    Reactor _reactor;
    ScopedWindow _window;

    IWindow* _focus = nullptr;

    struct DelayedAction{
        std::chrono::time_point<std::chrono::high_resolution_clock> triggerTime;
        std::function<void()> action;
    };
    std::vector< DelayedAction > _delayedActions;
};

}
