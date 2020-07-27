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
#include "Window.hpp"
#include "Log.hpp"
#include "Draw.hpp"
#include "Forward.hpp"
#include <stdexcept>
#include <limits>

namespace cursed{

Window::Window( Direction layout, const std::string& n, std::initializer_list<LayoutObject> children ) : 
    _layout( layout, children ), 
    _name(n) 
{
    _sizeLimits.maximum.width = std::numeric_limits<int>::max();
    _sizeLimits.maximum.height = std::numeric_limits<int>::max();
    _sizeLimits.minimum.width = 0;
    _sizeLimits.minimum.height = 0;

    int i = 0;
    for( auto child : _layout.objects() ) child->setParent( this, i );
}

Window::Window( Direction layout, const std::string& name ) : Window( layout, name, {} ) {}
Window::Window( Direction layout ) : Window( layout, "", {} ) {}
Window::Window( ) : Window( Direction::Vertical, "", {} ) {}

void Window::destroyChildren(){
    _layout.clear();
}

Window::~Window(){
    destroy();
}

int Window::layoutRatio() const{
    return _layoutRatio;
}
void Window::setLayoutRatio( int lr ) {
    int old = _layoutRatio;
    _layoutRatio = lr;
    if( lr != old ) signals.layoutRatioChanged.emit( lr, old );
}

void Window::destroy(){
}

IWindow* Window::childAt( const Point& p ) const{
    for( auto child : _layout.objects() ){ 
        if( child->dimensions().contains( p ) ){
            return child->childAt( dimensions().topLeft - p );
        }
    }

    return const_cast<Window*>(this);
}

Point Window::absolute( const Point& p ) const{
    if( parent() ){ return parent()->absolute( p ) + dimensions().topLeft; }

    return dimensions().topLeft + p;
}

void Window::draw( bool fullRefresh ) {

}

void Window::onMouseInput( const Point& relative, MouseButtonEvent& e ){
    for( IWindow* child : _layout.objects() ){
        child->onMouseInput( relative - child->dimensions().topLeft, e );
    }
}

void Window::setParent( IWindow* par, int& i ) {
    _id = ++i;
    _parent = par;

    if( _name.empty() ) { 
        std::stringstream newname; 
        if( parent() ) newname << parent()->name() << ":";
        newname << _id; 
        _name = newname.str(); 
    }
}

void Window::setDimensions( Rectangle rect ) {
    _dimensions = rect;
    refreshDimensions();
}

void Window::refreshDimensions(){
    _layout.onAllocationChanged( _dimensions );
    draw( true );
}

WINDOW* Window::window() {
    return stdscr;
}

void Window::refresh( bool fullRefresh ){
    draw( fullRefresh );
    for( IWindow* child : _layout.objects() ) child->refresh( fullRefresh );
}
}

