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
#include "Alignment.hpp"

namespace cursed{

class Label : public Window{
public:
    template< typename... Args >
    inline Label( std::string text,  Args&&... args ) : 
        Window( std::forward<Args>(args)... ),
        _text(text)
    { }

    typedef std::string NewValue;
    typedef std::string OldValue;
    typedef Signal< NewValue, OldValue > ValueChanged;

    struct {
        ValueChanged valueChanged; // pressed and released mouse 0 while cursor in window area
    } signals;

    void setColor( unsigned long color );

    void setText( const std::string txt );

    const std::string& text() const{ return _text; }

    void setAlignment( const Alignment& alignment );

    void draw( bool fullRefresh ) override;

private:
    std::string _text;
    struct{
        unsigned long color = 0;
        Alignment alignment = { HorizontalAlignment::Left, VerticalAlignment::Top};
    } visualProperties;
};

}
