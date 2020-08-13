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
#include "Window.hpp"
#include "Signal.hpp"
#include <functional>

namespace cursed{

enum class MouseButton : int {
    Button1 = 1,
    Button2 = 2,
    Button3 = 3,
    Left = Button1,
    Right = Button3,
    Center = Button2
};

class MouseEventWindow : public Window{
public:
    MouseEventWindow( Direction layout = Direction::Vertical, const std::string& name = "", std::initializer_list<LayoutObject> children = {} );

    MouseEventWindow( const MouseEventWindow& ) = delete;

    struct Pressed : public Signal<MouseButton, Point>{};
    struct Released : public Signal<MouseButton, Point>{};
    struct Clicked : public Signal<MouseButton, Point>{};

    struct MouseDrag : public Signal< MouseButton, Point signal_field(start), Point signal_field(current) >{};
    struct MouseMove : public Signal< Point signal_field(start), Point signal_field(current) >{};
    struct MouseEnter : public Signal<Point>{};
    struct MouseExit : public Signal<Point>{};

    struct : Window {
        Pressed pressed;
        Released released;
        Clicked clicked; // pressed and released mouse 0 while cursor in window area
        Clicked doubleClicked; // pressed and released mouse 0 while cursor in window area
        Clicked tripleClicked; // pressed and released mouse 0 while cursor in window area
        MouseDrag mouseDrag; // pressed and moved
        MouseMove mouseMove; // no-click hover/move
        MouseEnter mouseEnter;
        MouseExit mouseExit;
    } signals;

    void onMouseInput( const Point& relative, MouseButtonEvent& e ) override;

protected:
    bool mouseButtonPressed( MouseButton button );
    void onMouseButtonPressed( MouseButton button, bool contained, const Point& relativePos );
    void onMouseButtonReleased( MouseButton button, const Point& relativePos );
private:
    std::string _text;
    bool _pressed[4] = {false};
    bool _mouseContained = false;
    struct{
        unsigned long normalColor = 0;
        unsigned long pressedColor = 0;
        unsigned long attributes = 0;
    } visualProperties;
    Point _pressedPoint[4] = {};
    Point _lastPosition;
    Point _lastDrag[4] = {};
};

}
