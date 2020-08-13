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
#include "ILayout.hpp"

#include "IWindow.hpp"

#include "Point.hpp"
#include "Rectangle.hpp"
#include "Direction.hpp"
#include "LayoutObject.hpp"


#include <vector>

namespace cursed{
    class BoxLayout : public ILayout{
    public:
        BoxLayout( Direction dir, std::initializer_list<LayoutObject> objects = {} );
        ~BoxLayout();

        void append( int ratio_, IWindow* win );
        bool remove( IWindow* win );
        void clear();

        void onAllocationChanged( Rectangle allocation ) final;
        void updateLayout( ) final;

        const std::vector<IWindow*>& objects(){ return _objects; }
        const std::vector<IWindow*>& objects() const{ return _objects; }

        Direction direction() const{ return _dir; }

    private:
        Direction _dir = Direction::None;

        std::vector<IWindow*> _objects;
        int _ratioSum = 0;

        bool _debug = false;
        Rectangle _allocation;
    };
}

