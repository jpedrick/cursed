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
                {1, _valueIndicator    = new Button("")},
                {1, _belowValueButton  = new Button("")},
                {1, _increaseButton    = new Button( increasingArrow(layoutDirection) ) }
        } ),
        _layoutDirection( layoutDirection )
    { 
        {
            SizeLimits limits = sizeLimits();
            limits.minimum.getDimension( layoutDirection ) = 3; // one per button, one for value indicator.
            limits.minimum.getDimension( other(layoutDirection) ) = 1; 

            setSizeLimits( limits );
        }

        constexpr int actionButton = 1;

        auto decreaseValue = [&]( int ){
            cursed_out( "clicked decrease button: " << cprint(name) );
            incrementValue(-1); 
        };
        auto increaseValue = [&]( int ){
            cursed_out( "clicked increase button: " << cprint(name) );
            incrementValue(1); 
        };
        _decreaseButton->signals.clicked.connect( decreaseValue );
        _decreaseButton->signals.released.connect( decreaseValue );

        _increaseButton->signals.clicked.connect( increaseValue );
        _increaseButton->signals.released.connect( increaseValue );

        _aboveValueButton->signals.clicked.connect([&]( int mouseButton ){ incrementValue( -_buttonIncrement ); }, {actionButton} );
        _belowValueButton->signals.clicked.connect([&]( int mouseButton ){ incrementValue(  _buttonIncrement ); }, {actionButton} );

        _valueIndicator->signals.mouseDrag.connect( [&]( Point start, Point current ){
            int stVal = start.getPosition( layoutDirection );
            int curVal = current.getPosition( layoutDirection );

            int diff = curVal - stVal;
            int size = _valueIndicator->dimensions().size.getDimension( layoutDirection )
                 + _aboveValueButton->dimensions().size.getDimension( layoutDirection )
                 + _belowValueButton->dimensions().size.getDimension( layoutDirection );
            int64_t perCell = _maxValue / size;

            this->incrementValue( diff * perCell );
        } );
        {
            SizeLimits limits = _valueIndicator->sizeLimits();
            limits.minimum.width = 2;
            limits.minimum.height = 2;
            limits.maximum.getDimension( layoutDirection ) = 1000;
            _valueIndicator->setSizeLimits( limits );
        }
        {
            SizeLimits limits = _decreaseButton->sizeLimits();
            limits.minimum.width = 3;
            limits.minimum.height = 1;
            limits.maximum.getDimension( layoutDirection ) = 1;
            _decreaseButton->setSizeLimits( limits);
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
            _aboveValueButton->setLayoutRatio( _value );
            _belowValueButton->setLayoutRatio( _maxValue - _value );

            int size = _valueIndicator->dimensions().size.getDimension( _layoutDirection )
                 + _aboveValueButton->dimensions().size.getDimension( _layoutDirection )
                 + _belowValueButton->dimensions().size.getDimension( _layoutDirection );

            int64_t perCell = size > 0 ? _maxValue / size : 1;
            _valueIndicator->setLayoutRatio(perCell);

            layout().onAllocationChanged( this->dimensions() );
        }
    }

    void ScrollBar::refreshDimensions() {
        int space = (dimensions().size.getDimension(_layoutDirection) - 2);
        _valueIndicator->sizeLimits().minimum.getDimension( _layoutDirection) = 
            (double)space/(double)_maxValue;

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

