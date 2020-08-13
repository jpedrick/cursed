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
#include "Button.hpp"
#include "Draw.hpp"
#include <thread>

namespace cursed{

  Button::Button( std::string text, Direction layoutDirection, const std::string& name, std::initializer_list<LayoutObject> children ) : 
        MouseEventWindow( layoutDirection, name, children ),
        _text(text)
    { 
        connectActions();
        setBorderStyle( BoxStyle::Fancy2 );
    }

void Button::connectActions(){
    signals.pressed.connect([&]( MouseButton mouseButton, Point ){
        if( mouseButton == MouseButton::Button1 ){
            _pressed = true; draw(true); ::refresh();
        }
    });

    const auto setPressedAndRedraw = []( Button* button ){
        button->_pressed = true; button->draw(true); ::refresh(); 
    };
    const auto unsetPressedAndRedraw = []( Button* button ){
        button->_pressed = false; button->draw(true); ::refresh(); 
    };

    signals.clicked.connect([&]( MouseButton, Point ){ 
        setPressedAndRedraw( this );
        this->addDelayedAction( std::chrono::milliseconds(this->reactionDelay()),[&]{
            unsetPressedAndRedraw(this);
        }); 
    }, { MouseButton::Button1,{} } );

    signals.released.connect([&]( MouseButton, Point ){ 
        unsetPressedAndRedraw(this);
    }, {MouseButton::Button1,{}} );

    signals.doubleClicked.connect([&]( MouseButton, Point ){
        setPressedAndRedraw(this);
        addDelayedAction( std::chrono::milliseconds(this->reactionDelay()),[&]{
            unsetPressedAndRedraw(this);
        }); 
    }, {MouseButton::Left,{}} );

    signals.tripleClicked.connect([&]( MouseButton, Point ){
        setPressedAndRedraw(this);
        addDelayedAction( std::chrono::milliseconds(this->reactionDelay()),[&]{
            unsetPressedAndRedraw(this);
        }); 
    }, {MouseButton::Left,{}} );
}

int Button::reactionDelay() const{
    return _reactionDelay;
}

void Button::draw( bool fullRefresh ){
    if( fullRefresh ){
        unsigned long color = _pressed ? visualProperties.pressedColor : visualProperties.normalColor;
        Draw::AttributeGuard ag{ this, color | visualProperties.attributes };

        Rectangle dim = dimensions();

        if ( fullRefresh ){
            // clear button
            for( int y = 0; y < dim.size.height; ++y ){
                Draw::line( this, Direction::Horizontal, y, ' ');
            }
        }

        Rectangle boxDim{ {0,0}, dim.size };
        Draw::box( this, boxDim, borderStyle() );

        int midw = dim.size.width / 2;
        int midy = dim.size.height / 2;

        int midl = _text.length();

        int startx = std::max(0, midw - midl );
        Point labelPosition{ startx, midy };

        Draw::text( this, {startx,midy}, _text );
    }
}

}
