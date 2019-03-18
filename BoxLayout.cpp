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
#include "BoxLayout.hpp"
#include "ConsoleMode.hpp"

#include <algorithm>

namespace cursed{

BoxLayout::BoxLayout( Direction dir, std::initializer_list<LayoutObject> objects ) : 
        _dir(dir), _ratioSum(0) 
{
    for( auto o : objects ) append( o._ratio, o.window );
}

void BoxLayout::append( int ratio_, IWindow* win ){ 
    _objects.emplace_back( win ); 
    win->setLayoutRatio( ratio_ );
    _ratioSum += ratio_;

    win->signals.layoutRatioChanged.connect([&](int newRatio, int oldRatio){
       int change = newRatio - oldRatio;
       _ratioSum += change;
    } );
}

bool BoxLayout::remove( IWindow* win ){
    auto objectMatches = [win]( const IWindow* window ){ return window == win; };
    auto obj = std::find_if( _objects.begin(), _objects.end(), objectMatches ); 

    if( obj != _objects.end() ) {
        _ratioSum -= win->layoutRatio();
        _objects.erase( obj );
        return true;
    }

    return false;
}

BoxLayout::~BoxLayout(){
    _objects.clear();
}

void BoxLayout::clear(){
    for( auto obj : _objects ){ 
        delete obj; 
    }
    _objects.clear();
    _ratioSum = 0;
}

void BoxLayout::onAllocationChanged( Rectangle allocation ) {
    int layoutDimension = allocation.size.getDimension( _dir );
    int currentPosition = 0;
    int remainingSpace = layoutDimension;
    for( size_t i = 0; i < _objects.size(); ++i ){
        auto& obj = _objects[i];
        if( obj->show() ){
            auto d = ::div( obj->layoutRatio() * layoutDimension, _ratioSum );
            Rectangle& objectDimensions = obj->dimensions();

            objectDimensions.size.getDimension( _dir ) = 
                obj->sizeLimits().clippedSize( _dir, d.quot );

            objectDimensions.size.getDimension( other(_dir) ) = 
                obj->sizeLimits().clippedSize( other(_dir), allocation.size.getDimension( other(_dir) ) );

            objectDimensions.topLeft.getPosition( _dir ) = currentPosition;

            objectDimensions.topLeft.getPosition( other(_dir) ) = 0;

            currentPosition += objectDimensions.size.getDimension( _dir );

            remainingSpace -= objectDimensions.size.getDimension( _dir );
        }
    }
    bool hasAdjustable = false;
    if( remainingSpace > 0 ){
        std::vector<size_t> adjustableObjs;
        for( size_t i = 0; i < _objects.size(); ++i ){
            auto& obj = _objects[i];
            int maxSize = obj->sizeLimits().maximum.getDimension(_dir);
            int currentSize = obj->dimensions().size.getDimension(_dir );
            bool isAdjustable = maxSize > currentSize;
            if( isAdjustable ){
                adjustableObjs.push_back(i);
                hasAdjustable = true;
            }
        }
        if( adjustableObjs.size() > 0 ){
            int adjAmt = std::max( remainingSpace, remainingSpace / (int)adjustableObjs.size() );
            if( adjAmt > 0 ){
                for( size_t i : adjustableObjs ){
                    remainingSpace -= adjAmt;
                    auto& obj = _objects[i];
                    obj->dimensions().size.adjust(_dir, adjAmt );
                    if( i == 0 ){
                        int remainder = remainingSpace % adjAmt;
                        obj->dimensions().size.adjust( _dir, remainder );

                        remainingSpace -= remainder;
                    }
                    if( remainingSpace <= 0 ) break;
                }
            }
        }
    }

    {
        int currentPosition = 0;
        for( size_t i = 0; i < _objects.size(); ++i ){
            auto& obj = _objects[i];
            obj->dimensions().topLeft.getPosition(_dir) = currentPosition;
            currentPosition += obj->dimensions().size.getDimension(_dir);
            obj->refreshDimensions();
        }

        if( currentPosition != layoutDimension && hasAdjustable ){
            std::stringstream ss;
            ss << "bad layout " << cprint(currentPosition) << cprint(layoutDimension) << cprint(_objects.size());
            throw std::runtime_error(ss.str());
        }
    }
}

}
