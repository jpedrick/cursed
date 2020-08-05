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
#include "IWindow.hpp"
#include "Rectangle.hpp"
#include "BoxLayout.hpp"
#include "Signal.hpp"
#include "BoxStyle.hpp"

#include <vector>

namespace cursed{

class Window : public IWindow {
public:
    // primary constructor
    Window( Direction layout, const std::string& name, std::initializer_list<LayoutObject> children );

    Window( Direction layout, const std::string& name );
    Window( Direction layout );
    Window( );

    ~Window();

    void setParent( IWindow* parent, int& i ) final;

    IWindow* parent() final{ return _parent; }
    const IWindow* parent() const final{ return _parent; }

    int id() const final{ return _id; }

    void refresh( bool fullRefresh ) override;

    WINDOW* window() override;

    void onKeyboardInput( int unused(c) ) override{ }
    void onMouseInput( const Point& relative, MouseButtonEvent& e ) override;

    virtual void setBorderStyle( BoxStyle );
    virtual BoxStyle borderStyle( );

    virtual void setSizeLimits( const SizeLimits& limits ) override{ _sizeLimits = limits; }
    virtual const SizeLimits& sizeLimits() const override { return _sizeLimits; }
    virtual SizeLimits& sizeLimits() override { return _sizeLimits; }

    virtual void setName( const std::string& name ) override { _name = name; }
    virtual std::string name( ) const override { return _name; }

    void setDimensions( Rectangle rect ) final;
    const Rectangle& dimensions() const override { return _dimensions; }
    Rectangle& dimensions() override { return _dimensions; }
    void refreshDimensions() override;

    Point absolute( const Point& ) const override;
    IWindow* childAt( const Point& p ) const override;

    int layoutRatio() const override;
    void setLayoutRatio( int ) override;
    bool canAquireFocus() const override { return false; }
    void setDebug(){ _debug = true; }

protected: 
    void setDrawWindowDelegate( IDrawWindowDelegate* delegate ) override { _drawingDelegate = delegate; }
    IDrawWindowDelegate* drawWidowDelegate() override { return _drawingDelegate; }
    void draw( bool fullRefresh ) override;

    void destroyChildren();
    void destroy();
    BoxLayout& layout(){ return _layout; }
    bool debug() const override{ return _debug; }
    bool show() const override { return true; }

private:
    IWindow* _parent = nullptr;
    IDrawWindowDelegate* _drawingDelegate = nullptr;
    BoxLayout _layout;
    Rectangle _dimensions;
    SizeLimits _sizeLimits;
    int _id = -1;
    std::string _name;
    int _layoutRatio = 1;
    bool _debug = false;
    BoxStyle _borderStyle = BoxStyle::None;
};

}
