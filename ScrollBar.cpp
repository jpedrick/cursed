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
#include "ScrollBar.hpp"
#include "Button.hpp"
#include "ProgressBar.hpp"
#include "UTF.hpp"
#include "Log.hpp"
#include "Forward.hpp"

namespace cursed{
    std::string increasingArrow( Direction dir ){
        if( dir == Direction::Horizontal ){
            return "🡆";
        }

        return "🡇";
    }

    std::string decreasingArrow( Direction dir ){
        if( dir == Direction::Horizontal ){
            return "🡄";
        }

        return "🡅";
    }

    ScrollBar::ScrollBar( Direction layoutDirection , const std::string& name ) : 
        Window( layoutDirection, name, {
                {1, _decreaseButton    = new Button( decreasingArrow(layoutDirection) ) },
                {20, _valueIndicator    = new ProgressBar(layoutDirection, {}, "value-indicator:" + name)},
                {1, _increaseButton    = new Button( increasingArrow(layoutDirection) ) }
        } ),
        _layoutDirection( layoutDirection )
    { 
        _decreaseButton->setBorderStyle( BoxStyle::None );
        _increaseButton->setBorderStyle( BoxStyle::None );
        {
            SizeLimits limits = sizeLimits();
            limits.minimum.getDimension( layoutDirection ) = 3; // one per button, one for value indicator.
            limits.minimum.getDimension( other(layoutDirection) ) = 1; 

            setSizeLimits( limits );
        }

        constexpr auto actionButton = MouseButton::Left;
        Button::Clicked::optional_filter_tuple leftButtonFilter{ MouseButton::Left, {} };

        // double and triple click events allow fast clicking the scroll buttons
        auto decreaseValue = [&]( MouseButton, Point ){ incrementValue(-1); };
        auto increaseValue = [&]( MouseButton, Point ){ incrementValue(1); };

        auto largeDecrease = [&](  MouseButton, Point ){ incrementValue( -_buttonIncrement ); };
        auto doubleLargeDecrease = [&]( MouseButton, Point ){ incrementValue( -_buttonIncrement * 2 ); };
        auto tripleLargeDecrease = [&]( MouseButton, Point ){ incrementValue( -_buttonIncrement * 3 ); };

        auto largeIncrease = [&]( MouseButton, Point ){ incrementValue( _buttonIncrement ); };
        auto doubleLargeIncrease = [&]( MouseButton, Point ){ incrementValue( _buttonIncrement * 2 ); };
        auto tripleLargeIncrease = [&]( MouseButton, Point ){ incrementValue( _buttonIncrement * 3 ); };

        _decreaseButton->signals.clicked.connect( decreaseValue, leftButtonFilter );
        _decreaseButton->signals.doubleClicked.connect( [&]( MouseButton, Point ){ incrementValue(-2); }, leftButtonFilter );
        _decreaseButton->signals.tripleClicked.connect( [&]( MouseButton, Point ){ incrementValue(-3); }, leftButtonFilter );
        _decreaseButton->signals.released.connect( decreaseValue, leftButtonFilter );

        _increaseButton->signals.clicked.connect( increaseValue, {actionButton, {}}  );
        _increaseButton->signals.doubleClicked.connect( [&]( MouseButton, Point ){ incrementValue(2); }, leftButtonFilter );
        _increaseButton->signals.tripleClicked.connect( [&]( MouseButton, Point ){ incrementValue(3); }, leftButtonFilter );
        _increaseButton->signals.released.connect( increaseValue, leftButtonFilter );

        _valueIndicator->progressBarSignals.clicked.connect( [&]( ProgressBar::Area area, MouseButton ){
            incrementValue(-1);
        }, { ProgressBar::Area::Value, MouseButton::Left } );

        _valueIndicator->progressBarSignals.clicked.connect( [&]( ProgressBar::Area area, MouseButton ){
            incrementValue(1);
        }, ProgressBar::Clicked::optional_filter_tuple{ ProgressBar::Area::Remaining, MouseButton::Left } );

        {
            SizeLimits limits = _decreaseButton->sizeLimits();
            limits.minimum.width = 3; // arrows are wide
            limits.minimum.height = 1;
            limits.maximum.getDimension( layoutDirection ) = 3;
            _decreaseButton->setSizeLimits( limits );
        }
        {
            SizeLimits limits = _increaseButton->sizeLimits();
            limits.minimum.width = 3; // arrows are wide
            limits.minimum.height = 1;
            limits.maximum.getDimension( layoutDirection ) = 3;
            _increaseButton->setSizeLimits( limits );
        }
        update();
    }

    void ScrollBar::update( bool force ){
        if( force ){
            layout().onAllocationChanged( this->dimensions() );
        }
    }

    void ScrollBar::refreshDimensions() {
        Window::refreshDimensions();
    }

    int64_t ScrollBar::incrementValue( int64_t change ){
        return setValue( value() + change );
    }

    int64_t ScrollBar::setValue( int64_t newValue ){ 
        int64_t oldValue = _value;
        _value = std::max(0L, std::min(newValue, _maxValue)); 
        {
            auto signalBlock = _valueIndicator->progressBarSignals.clicked.block();
            _valueIndicator->setValue( newValue );
        }

        if( _value != oldValue ){ 
            signals.valueChanged.emit( _value, oldValue );
            update();
        }

        return _value;
    }

    void ScrollBar::setMaxValue( int64_t newMax ) { 
        int oldValue = _maxValue;
        _maxValue = std::max(0L, newMax ); 
        _valueIndicator->setValueRange({0, newMax});
        if( _maxValue != oldValue ){
            update( true );
        }
    }

    void ScrollBar::setIndicatorColors( unsigned long value, unsigned long remaining ){
        _valueIndicator->setColors( value, remaining);
    }

    void ScrollBar::setButtonColors( unsigned long normal, unsigned long pressed ){
        _decreaseButton->setColors( normal, pressed );
        _increaseButton->setColors( normal, pressed );
    }
}

