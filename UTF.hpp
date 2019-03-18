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
class UTF16{
public:
    static constexpr char BallotBox[][4] = { 
        "\u2610", //Unchecked
        "\u2611", //Checked
        "\u2612"  //With X
    };

    static constexpr char Dice[][4] = { 
        "\u2680", 
        "\u2681", 
        "\u2682", 
        "\u2683", 
        "\u2684",
        "\u2685"
    };

    static constexpr char blackArrow[][4] = {
        "\u2B05", // Left
        "\u27A1", // Right
        "\u2B06", // Up
        "\u2B07", // Down
        "\u2B08", // UpRight
        "\u2B09", // UpLeft
        "\u2B0A", // DownRight
        "\u2B0B", // DownLeft
        "\u2B0C", // UpDown
        "\u2B0D", // RightLeft
    };
    enum class BlackArrow : int{
        Left = 0,
        Right = 1,
        Up = 2,
        Down = 3,
        UpRight = 4,
        UpLeft = 5,
        DownRight = 6,
        DownLeft = 7,
        UpDown = 8,
        RightLeft = 9
    };

    static constexpr char Braille[][4] = {
        "\u2800", //Blank
        "\u2847", //Full Left
        "\u28B8", //Full Right
        "\u28FF"  //Full-2x4
    };
};
}
