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
#include "MouseEventWindow.hpp"
#include "Signal.hpp"
#include <functional>

namespace cursed{
class Button;

class ScrollBar : public Window{
public:
    typedef int64_t NewValue;
    typedef int64_t OldValue;
    typedef Signal< NewValue, OldValue > ValueChanged;

    struct {
        ValueChanged valueChanged; // pressed and released mouse 0 while cursor in window area
    } signals;

    ScrollBar( Direction layout, const std::string& name = "" );

//    void draw( bool fullRefresh ) override;
    int64_t setValue( int64_t v );
    void setMaxValue( int64_t v );
    int64_t incrementValue( int64_t change );

    int64_t value() const{ return _value; }
    int64_t maxValue() const{ return _maxValue; }
    double percent() const { return (double)_value / (double)_maxValue; }

    void update();
    void setButtonIncrement( int v ) { _buttonIncrement = v; }
    void setButtonColors( unsigned long normal, unsigned long pressed );
    void setIndicatorColors( unsigned long normal, unsigned long pressed );

    void setShow( bool val ){ _show = val; }
    bool show() const override{ return _show; }

    void refreshDimensions() override;

    Button& increaseButton();
    Button& decreaseButton();

private:
    Button* _decreaseButton;
    Button* _aboveValueButton;
    Button* _valueIndicator;
    Button* _belowValueButton;
    Button* _increaseButton;

    int64_t _value;
    int64_t _maxValue;
    int64_t _buttonIncrement = 1;
    Direction _layoutDirection;
    bool _show = true;

};

}

