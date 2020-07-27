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
#include "Rectangle.hpp"

inline bool contains( const cursed::Rectangle& rect, const cursed::Point& p ) {
    using namespace cursed;
    int relx = p.x - rect.topLeft.x;
    int rely = p.y - rect.topLeft.y;

    bool xOk = ( relx >= 0 && relx < rect.size.width );
    bool yOk = ( rely >= 0 && rely < rect.size.height );

    cursed_out( cprint(xOk) << cprint(yOk) << cprint(relx) << cprint(rely) << cprint(p.y) << cprint( rect.topLeft.y)  );

    return xOk &&  yOk;
}

int main( int, char* [] ){
    using namespace cursed;

    Rectangle tr{ {0,0}, { 11, 10 } };

    expect( tr.contains( {0,0} ) );
    expect( !tr.contains( {-1,0} ) );
    expect( !tr.contains( {0,-1} ) );
    expect( !tr.contains( {-1,-1} ) );
    expect( !tr.contains( {11,10} ) );

    expect( tr.contains( {10,9} ) );
    expect( !tr.contains( {10,10} ) );
    expect( !tr.contains( {11,9} ) );

    Rectangle tr2{ {0,12}, {62,13} };

    cursed_out( cprint(tr2) );
    expect( contains( tr2, {29,6} ) );
    expect( tr2.contains( {29,6} ) );

    return 0;
}
