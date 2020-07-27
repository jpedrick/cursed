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
    }

void Button::connectActions(){
    signals.pressed.connect([&]( int mouseButton ){
        if( mouseButton == 1 ){
            _pressed = true; draw(true); ::refresh();
        }
    });

    const auto setPressedAndRedraw = []( Button* button ){
        button->_pressed = true; button->draw(true); ::refresh(); 
    };
    const auto unsetPressedAndRedraw = []( Button* button ){
        button->_pressed = false; button->draw(true); ::refresh(); 
    };

    constexpr int leftButton = 1;
    signals.clicked.connect([&]( int mouseButton ){ 
        setPressedAndRedraw( this );
        addDelayedAction( std::chrono::milliseconds(200),[&]{
            unsetPressedAndRedraw(this);
        }); 
    }, {leftButton} );

    signals.released.connect([&]( int mouseButton ){ 
        unsetPressedAndRedraw(this);
    }, {leftButton} );

    signals.doubleClicked.connect([&]( int mouseButton ){
        setPressedAndRedraw(this);
        addDelayedAction( std::chrono::milliseconds(200),[&]{
            unsetPressedAndRedraw(this);
        }); 
    }, {leftButton} );

    signals.tripleClicked.connect([&]( int mouseButton ){
        setPressedAndRedraw(this);
        addDelayedAction( std::chrono::milliseconds(200),[&]{
            unsetPressedAndRedraw(this);
        }); 
    }, {leftButton} );
}

void Button::draw( bool fullRefresh ){
    if( fullRefresh ){
        unsigned long color = _pressed ? visualProperties.pressedColor : visualProperties.normalColor;
        Draw::AttributeGuard ag{ this, color | visualProperties.attributes };

        Rectangle dim = dimensions();

        for( int y = 0; y < dim.size.height; ++y ){
            Draw::line( this, Direction::Horizontal, y, ' ');
        }

        int midw = dim.size.width / 2;
        int midy = dim.size.height / 2;

        int midl = _text.length();

        int startx = std::max(0, midw - midl );
        Point labelPosition{ startx, midy };

        Draw::text( this, {startx,midy}, _text );
    }
}

}
