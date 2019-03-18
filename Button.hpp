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
class Button : public Window{
public:
    typedef Signal< > Pressed;
    typedef Signal< > Released;
    typedef Signal< > Clicked;
    typedef Signal< Point signal_field(start), Point signal_field(current) > MouseDrag;

    struct : Window {
        Pressed pressed;
        Released released;
        Clicked  clicked; // pressed and released mouse 0 while cursor in window area
        Clicked  doubleClicked; // pressed and released mouse 0 while cursor in window area
        Clicked  tripleClicked; // pressed and released mouse 0 while cursor in window area
        MouseDrag mouseDrag;
    } signals;

    template< typename... Args >
    inline Button( std::string text,  Args&&... args ) : 
        Window( std::forward<Args>(args)... ),
        _text(text)
    { 
    }

    void onMouseInput( const Point& relative, MouseButtonEvent& e ) override;
    void draw( bool fullRefresh ) override;

    void setColors( unsigned long normalColor, unsigned long pressedColor ){
        visualProperties.normalColor = normalColor;
        visualProperties.pressedColor = pressedColor;
    }

    void setAttributes( unsigned long attrs ){ visualProperties.attributes = attrs; }
    void setDebug(){ _debug = true; }
    void setText( const std::string& tx ){ _text = tx; }
protected:
    bool debug() const override{ return _debug; }
private:
    std::string _text;
    bool _pressed = false;
    int _pressedButton = 0;
    struct{
        unsigned long normalColor = 0;
        unsigned long pressedColor = 0;
        unsigned long attributes = 0;
    } visualProperties;
    bool _debug = false;
    Point _pressedPoint;
};

}
