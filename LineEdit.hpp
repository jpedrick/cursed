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

class LineEdit: public Window{
public:
    struct{
        Signal< std::string > textChanged;
        Signal< >             returnPressed;
        Signal< >             editingFinished; // Lost focus while editing
    } signals;

    template< typename... Args > inline LineEdit( Args&&... args ) : Window( std::forward<Args>(args)... ) { 
        static_cast<Window*>(this)->signals.acquiredFocus.connect([&]{ 
            if( !_hasFocus ){
                _hasFocus = true; 
                draw(true); ::refresh();
            }
        });
        static_cast<Window*>(this)->signals.lostFocus.connect([&]{ 
            if( _hasFocus ){
                _hasFocus = false; 
                draw(true); ::refresh();
            }
        });
        {
            SizeLimits szLim = sizeLimits();
            szLim.minimum.height = 1;
            szLim.maximum.height = 1;
            setSizeLimits( szLim );
        }
    }

    void draw( bool fullRefresh ) override;
    void setText( const std::string& newText );
    const std::string text() const{ return _text; }
    void setCanAquireFocus( bool can ) { _canAcquireFocus = can; }
    bool canAquireFocus() const override { return _canAcquireFocus; }
    bool hasFocus() const { return _hasFocus; }
    void onKeyboardInput( int c ) override;

private:
    void deleteLeft();
    void deleteRight();
    void moveCursor( int increment );
    void insert( int c );

    int _cursorPosition = 0;
    std::string _text;
    bool _canAcquireFocus = true;
    bool _hasFocus = false;
};

}
