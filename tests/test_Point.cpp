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
#include "Test.hpp"
#include "Point.hpp"

int main( int, char* [] ){
    using namespace cursed;

    Point p1{ 1, 1, 1 };
    Point p2{ 2, 2, 2 };

    Point p3 = p1 - p2;
    expect( p3.x == -1 );
    expect( p3.y == -1 );
    expect( p3.z == -1 );

    Point p4{ 35, 3, 0 };
    Point p5{ 0, 15, 0 };

    expect( ( p4 - p5 ).y == -12 );
 
    Point p6{16,4,0}; 
    Point p7{0,10,0};
    expect( ( p6 - p7 ).y == -6 );
    cursed_out( cprint(p6-p7) );

    return 0;
}
