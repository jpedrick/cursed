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
#include "LineEdit.hpp"
#include "Draw.hpp"
#include "Application.hpp"

namespace cursed{
    void LineEdit::setup(){
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

    void LineEdit::draw( bool fullRefresh ) {
        if( fullRefresh ){
            Rectangle dim = dimensions();
            {
                Draw::AttributeGuard extended{ this, A_ALTCHARSET };
                Draw::line( this, Direction::Horizontal, 0, ACS_CKBOARD );
            }

            int maxWidth = std::max(0,dim.size.width - 1);
            if( _cursorPosition < dim.size.width ){
                maxWidth = dim.size.width;
            }
            int firstChar = _cursorPosition < maxWidth ? 0 : _cursorPosition - maxWidth;
            int displayedWidth = std::min( (int)_text.size() - firstChar, maxWidth );
            {
                std::string subText = _text.substr( firstChar, displayedWidth );
                Draw::text( this, {0,0}, subText );
            }

            // indicate more text to left
            if( firstChar > 0 ){
                Draw::AttributeGuard underline{ this, A_BOLD | A_UNDERLINE | A_DIM };
                Draw::character( this, { 0, 0 }, _text[firstChar] );
            }
            int lastDisplayedChar = firstChar + displayedWidth - 1;

            // indicate more text to right
            if( _cursorPosition < lastDisplayedChar && (int)_text.size() > dim.size.width ){
                Draw::AttributeGuard underline{ this, A_BOLD | A_UNDERLINE | A_DIM };
                Draw::character( this, { displayedWidth - 1, 0 }, _text[lastDisplayedChar] );
            }

            if( _hasFocus )
            {
                if( _cursorPosition < (int)_text.size() && _cursorPosition >= 0 ){
                    Draw::AttributeGuard revrs{ this, A_REVERSE };
                    Draw::character( this, { _cursorPosition - firstChar, 0 }, _text[_cursorPosition] );
                }else{
                    Draw::AttributeGuard extended_blink{ this, A_REVERSE };
                    Draw::character( this, { std::max(0,_cursorPosition - firstChar), 0 }, '_' );
                }
            }
        }
    }

    void LineEdit::setText( const std::string& newText ){ 
        _text = newText; 
        _cursorPosition = _text.size();
        lineEditSignals.textChanged.emit( _text );
        draw( true );
        ::refresh();
    }

    void LineEdit::deleteLeft(){
        if( _cursorPosition > 0 && _text.size() > 0 ){
            int erasePos = std::max( 0, _cursorPosition - 1 );
            _text.erase( erasePos, 1 );
            _cursorPosition = erasePos;
            draw( true ); ::refresh();
        }
    }
    void LineEdit::deleteRight(){
        if( _cursorPosition < (int)_text.size() ){
            int erasePos = std::min( _cursorPosition, (int)_text.size() );
            if( (int)_text.size() < erasePos ){ 
                _text.erase( erasePos, 1 );
                if( erasePos >= (int)_text.size() ){
                    moveCursor( -1 );
                }
            }

            draw( true ); ::refresh();
        }
    }
    void LineEdit::moveCursor( int increment ){
        _cursorPosition = std::max( 0, std::min( _cursorPosition + increment, (int)_text.size() ) );
    }

    void LineEdit::insert( int c ){
        int pos = std::max( 0, _cursorPosition++ );
        _text.insert( _text.begin() + pos, c ); 
        draw( true ); ::refresh();
    }

    void LineEdit::onKeyboardInput( int c ){
        switch( c ){
            /// Eventually handle shift-down, move left/right to select text. ctrl-c ctrl-v to cut/paste from clipboard
            case KEY_BACKSPACE: {
                deleteLeft(); 
                return;
            }
            case KEY_DC:{
                deleteRight();
                return;
            }
            case KEY_LEFT:{
                moveCursor( -1 ); 
                draw( true ); ::refresh();
                return;
            }
            case KEY_RIGHT:{
                moveCursor( 1 ); 
                draw( true ); ::refresh();
                return;
            }
            case 13: {
                lineEditSignals.returnPressed.emit();
                return;
            }
        }

        if( std::isprint( c )  ){
            insert(c);
        }
    }
}

