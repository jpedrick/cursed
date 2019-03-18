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

namespace cursed{

struct Border{
    enum class Line: unsigned char {
        Left   = 179,
        Right  = 179,
        Top    = 196,
        Bottom = 196,
        TopLeft = 218,
        TopRight = 191,
        BottomLeft = 192,
        BottomRight = 217
    };
    enum class Simple3D: unsigned char {
        Left   = 179,
        Right  = 186,
        Top    = 196,
        Bottom = 205,
        TopLeft = 218,
        TopRight = 183,
        BottomLeft = 212,
        BottomRight = 189
    };
    enum class Double: unsigned char {
        Left   = 179,
        Right  = 186,
        Top    = 196,
        Bottom = 205,
        TopLeft = 218,
        TopRight = 183,
        BottomLeft = 212,
        BottomRight = 189
    };

    enum class Style{
        Default,
        None,
        Hash,
        Line,
        Simple3D,
        Double
    };
};
}
