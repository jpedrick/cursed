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
                {1, _aboveValueButton  = new Button("")},
                {1, _valueIndicator    = new Button("o", layoutDirection, "value-indicator:" + name)},
                {1, _belowValueButton  = new Button("")},
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

        _aboveValueButton->signals.clicked.connect(largeDecrease, leftButtonFilter );
        _aboveValueButton->signals.doubleClicked.connect(doubleLargeDecrease, leftButtonFilter );
        _aboveValueButton->signals.tripleClicked.connect(tripleLargeDecrease, leftButtonFilter );
        _aboveValueButton->signals.released.connect(largeDecrease, leftButtonFilter );

        _belowValueButton->signals.clicked.connect(largeIncrease, leftButtonFilter );
        _belowValueButton->signals.doubleClicked.connect(doubleLargeIncrease, leftButtonFilter );
        _belowValueButton->signals.tripleClicked.connect(tripleLargeIncrease, leftButtonFilter );
        _belowValueButton->signals.released.connect(largeIncrease, leftButtonFilter );

        _valueIndicator->signals.mouseDrag.connect( [&]( MouseButton button, Point start, Point current ){
            int stVal = start.getPosition( layoutDirection );
            int curVal = current.getPosition( layoutDirection );

            int diff = curVal - stVal;
            int size = _valueIndicator->dimensions().size.getDimension( layoutDirection )
                 + _aboveValueButton->dimensions().size.getDimension( layoutDirection )
                 + _belowValueButton->dimensions().size.getDimension( layoutDirection );
            int64_t perCell = _maxValue / size;

            this->incrementValue( diff * perCell );
        }, { MouseButton::Left, {}, {} } );
        {
            SizeLimits limits = _valueIndicator->sizeLimits();
            limits.minimum.width = 1;
            limits.minimum.height = 1;
            limits.maximum.getDimension( layoutDirection ) = 1000;
            _valueIndicator->setSizeLimits( limits );
        }
        {
            SizeLimits limits = _decreaseButton->sizeLimits();
            limits.minimum.width = 3;
            limits.minimum.height = 1;
            limits.maximum.getDimension( layoutDirection ) = 1;
            _decreaseButton->setSizeLimits( limits );
        }
        {
            SizeLimits limits = _increaseButton->sizeLimits();
            limits.minimum.width = 3;
            limits.minimum.height = 1;
            limits.maximum.getDimension( layoutDirection ) = 1;
            _increaseButton->setSizeLimits( limits );
        }
        update();
    }

    void ScrollBar::update( bool force ){
        if( _value != _aboveValueButton->layoutRatio() || _belowValueButton->layoutRatio() != (_maxValue - _value) || force ){
            int64_t size = dimensions().size.getDimension( _layoutDirection ) - _increaseButton->dimensions().size.getDimension( _layoutDirection ) * 2;
            int64_t perCell = size > 0 ? _maxValue / size : 1L;

            _aboveValueButton->setLayoutRatio( std::max( _value - perCell / 2, 0L ) );
            _belowValueButton->setLayoutRatio( std::max( _maxValue - _value - perCell / 2, 0L ) );

            _valueIndicator->setLayoutRatio( std::max( perCell, 2L ) );

            layout().onAllocationChanged( this->dimensions() );
        }
    }

    void ScrollBar::refreshDimensions() {
        int space = (dimensions().size.getDimension(_layoutDirection) - 2);
        _valueIndicator->sizeLimits().minimum.getDimension( _layoutDirection) = 
            std::max( space/_maxValue, 1L );

        Window::refreshDimensions();
    }

    int64_t ScrollBar::incrementValue( int64_t change ){
        return setValue( value() + change );
    }

    int64_t ScrollBar::setValue( int64_t newValue ){ 
        int64_t oldValue = _value;
        _value = std::max(0L, std::min(newValue, _maxValue)); 

        if( _value != oldValue ){ 
            signals.valueChanged.emit( _value, oldValue );
            update();
        }

        return _value;
    }

    void ScrollBar::setMaxValue( int64_t newMax ) { 
        int oldValue = _maxValue;
        _maxValue = std::max(0L, newMax ); 
        if( _maxValue != oldValue ){
            update( true );
        }
    }

    void ScrollBar::setIndicatorColors( unsigned long normal, unsigned long pressed ){
        _valueIndicator->setColors( normal, pressed );
        _aboveValueButton->setColors( pressed, normal );
        _belowValueButton->setColors( pressed, normal );
    }

    void ScrollBar::setButtonColors( unsigned long normal, unsigned long pressed ){
        _decreaseButton->setColors( normal, pressed );
        _increaseButton->setColors( normal, pressed );
    }
}

