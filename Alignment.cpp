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
#include "Alignment.hpp"
#include "Log.hpp"

namespace cursed{

Log& operator<<( Log& l, HorizontalAlignment alignment ){
    switch(alignment){
        case HorizontalAlignment::Left: return l << "left";
        case HorizontalAlignment::Right: return l << "right";
        case HorizontalAlignment::Center: return l << "center";
    }

    return l << "*";
}
Log& operator<<( Log& l, VerticalAlignment alignment ){
    switch(alignment){
        case VerticalAlignment::Top: return l << "top";
        case VerticalAlignment::Bottom: return l << "bottom";
        case VerticalAlignment::Center: return l << "center";
    }

    return l << "*";
}

Log& operator<<( Log& l, const Alignment& alignment ){
    return l << "{ horizontal:" << alignment.horizontal << " veritcal: " << alignment.vertical << "}";
}

}
