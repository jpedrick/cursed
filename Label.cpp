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
#include "Label.hpp"
#include "Draw.hpp"
#include <algorithm>

namespace cursed{

void Label::draw( bool fullRefresh ){
    if( fullRefresh ){
        int startx = 0;
        int starty = 0;
        Rectangle dim = dimensions();
        
        Draw::AttributeGuard ag{ this, visualProperties.color };

        for( int y = 0; y < dim.size.height; ++y ){
            Draw::AttributeGuard ag{ this, A_ALTCHARSET };
            Draw::line( this, Direction::Horizontal, y, ACS_CKBOARD);
        }

        if( visualProperties.alignment.horizontal == HorizontalAlignment::Right ){
            startx = std::max( 0, dim.size.width - (int)_text.length() );
        }else if( visualProperties.alignment.horizontal == HorizontalAlignment::Center ){
            int midw = dim.size.width / 2;
            int midl = _text.length();

            startx = std::max(0, midw - midl );
        }

        if( visualProperties.alignment.vertical == VerticalAlignment::Bottom ){
            starty = dim.size.height - 1;
        }else if( visualProperties.alignment.vertical == VerticalAlignment::Center ){
            starty = dim.size.height / 2;
        }
        Draw::text( this, {startx,starty}, _text );
        ::refresh();
    }
}

void Label::setText( const std::string txt ){
    auto previousValue = _text;
    _text = txt;
    draw(true);
    signals.valueChanged.emit( _text, previousValue );
}

void Label::setColor( unsigned long color ){
    visualProperties.color = color;
}

void Label::setAlignment( const Alignment& alignment ){
    visualProperties.alignment = alignment;
}

}
