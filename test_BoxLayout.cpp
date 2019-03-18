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
#include <iostream>
#include "Log.hpp"
#include "Forward.hpp"
#include "Window.hpp"
#include "BoxLayout.hpp"
#include "Test.hpp"


int main( int argc, char* [] ){
    using namespace cursed;
    Log::_outputStream = &std::cout;

    cursed_out( cprint(argc) );

    Window win1, win2;

    BoxLayout testLayout{ Direction::Horizontal, 
        {
            {1, &win1}, 
            {1, &win2}
        } 
    };

    testLayout.onAllocationChanged( Rectangle{ Point{0,0,0}, Size{ 2, 1 } } );

    expect( win1.dimensions().topLeft.x == 0 );
    expect( win1.dimensions().topLeft.y == 0 );
    expect( win1.dimensions().size.width == 1 );
    expect( win1.dimensions().size.height == 1 );

    expect( win2.dimensions().topLeft.x == 1 );
    expect( win2.dimensions().topLeft.y == 0 );
    expect( win2.dimensions().size.width == 1 );
    expect( win2.dimensions().size.height == 1 );

    testLayout.remove( &win1 );
    testLayout.remove( &win2 );
    testLayout.clear();
    
    testLayout.onAllocationChanged( Rectangle{ Point{1,1,0}, Size{ 3, 2 } } );

    expect( win1.dimensions().topLeft.x == 1 );
    expect( win1.dimensions().topLeft.y == 1 );
    expect( win1.dimensions().size.width == 1 );
    expect( win1.dimensions().size.height == 2 );

    expect( win2.dimensions().topLeft.x == 2 );
    expect( win2.dimensions().topLeft.y == 1 );
    expect( win2.dimensions().size.width == 2 );
    expect( win2.dimensions().size.height == 2 );

    return 0;
}
