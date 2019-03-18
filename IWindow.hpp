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

#include "MouseButtonEvent.hpp"
#include "Rectangle.hpp"
#include "Border.hpp"
#include "SizeLimits.hpp"
#include "Signal.hpp"

#include <chrono>

extern "C"{
    struct _win_st;
    typedef struct _win_st WINDOW;
}

namespace cursed{

class IDrawWindowDelegate;

class IWindow{
public:
    struct {
        Signal<int signal_field(newRatio), int  signal_field(oldRatio)> layoutRatioChanged;
        Signal<> lostFocus;
        Signal<> acquiredFocus;
    } signals;

    virtual ~IWindow(){}
    virtual IWindow* parent() = 0;
    virtual const IWindow* parent() const = 0;
    virtual Point absolute( const Point& p ) const = 0;
    virtual IWindow* childAt( const Point& p ) const = 0;

//    virtual void setBorderStyle( Border::Style bs ) = 0;
    virtual void setParent( IWindow*, int& i ) = 0;
    virtual void refresh( bool fullRefresh ) = 0;
    virtual void draw( bool __attribute__((unused)) fullRefresh ){}

    virtual void setDimensions( Rectangle rect ) = 0;
    virtual const Rectangle& dimensions() const = 0;
    virtual Rectangle& dimensions() = 0;
    virtual void refreshDimensions() = 0;

    virtual void setSizeLimits( const SizeLimits& limits ) = 0;
    virtual const SizeLimits& sizeLimits() const = 0;
    virtual SizeLimits& sizeLimits() = 0;

    virtual int id() const = 0;

    virtual WINDOW* window() = 0;

    virtual void onKeyboardInput( int c ) = 0;
    virtual void onMouseInput( const Point& relative, MouseButtonEvent& event ) = 0;

    virtual void setName( const std::string& ) {}
    virtual std::string name( ) const { return ""; }

    virtual int layoutRatio() const = 0;
    virtual void setLayoutRatio( int ) = 0;

    virtual bool canAquireFocus() const =  0;

    virtual bool show() const = 0;
    virtual void addDelayedAction( std::chrono::milliseconds delay, std::function<void()> action ){
        if( parent() ){
            parent()->addDelayedAction( delay, action );
        }
    }

protected:
    virtual void setDrawWindowDelegate( IDrawWindowDelegate* ) = 0;
    virtual IDrawWindowDelegate* drawWidowDelegate() = 0;
    virtual bool debug() const = 0;
};


}
