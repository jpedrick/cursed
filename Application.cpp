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
#include "Application.hpp"

#include "IFileEventHandler.hpp"
#include "Reactor.hpp"
#include "Log.hpp"
#include "Timer.hpp"

extern "C"{
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
}

#include <memory>
#include <thread>
#include <chrono>
#include <stdexcept>

namespace cursed{

    static WINDOW* initScreen(){
        std::cout << "\x1b[?1006h";
        ::setlocale(LC_ALL, "");

        WINDOW* scr = ::initscr();
        ::curs_set(0);
        ::start_color();
        ::cbreak(); 
        ::noecho(); 
        ::raw();
        ::nonl();
        ::leaveok( scr, true );
        ::scrollok( scr, false );
        mmask_t oldEvents;
        ::keypad( scr, true );
        ::mousemask( ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, &oldEvents );

        return scr;
    }

    class STDINFileEventHandler : public IFileEventHandler{
    public:
        STDINFileEventHandler( IWindow* window ) : _window(window) {}
        int fileDescriptor() const{ return STDIN_FILENO; }
        void onEvent( int ) final{
            int c = ERR;
            c = ::getch();
            if( c > 0 ) _window->onKeyboardInput( c );
        }
    private:
        IWindow* _window;
    };

    static Application* globalApplication = nullptr;

    Application& Application::instance(){ return *globalApplication; }

    void resizeHandler(int){
        globalApplication->onRootResized();
    }

    Application::Application( const std::string& name, Direction dir, std::initializer_list<LayoutObject> children ) : 
        Window( dir, name, children ),
        _window( initScreen() )
    {
        globalApplication = this;

        int i = 0;
        setParent(nullptr,i);
        for( auto child : layout().objects() ) child->setParent( this, i );

        signal( SIGWINCH, resizeHandler );
    }

    Application::~Application(){
        Window::destroyChildren();
        destroy();
        ::endwin(); 
    }

    void Application::start(){
        raw();
        noecho();

        clear();
        onRootResized();
        Window::refresh( true );
        run();
    }

    void Application::onKeyboardInput( int c ){
        if( c == 3 ){ 
            _exit = true; 
        } else if( c == KEY_MOUSE ) {
            MouseButtonEvent mouseEvent{c};
            onMouseInput( mouseEvent.position(), mouseEvent );
            return;
        } else if( focus() ){
            focus()->onKeyboardInput( c );
        }
    }

    void Application::onMouseInput( const Point& relPos, MouseButtonEvent& event ){
        Window::onMouseInput( relPos, event );

        // TODO: consider making elements aquire focus themselves instead of handling at application level
        if( event.mouseKeyEvent( 1, MouseButtonEventType::Released ) || event.mouseKeyEvent( 1, MouseButtonEventType::Clicked )){
            IWindow* clickedWindow = this->childAt( event.position() );
            while( clickedWindow ){
                if( clickedWindow && clickedWindow->canAquireFocus() ){
                    return setFocus( clickedWindow );
                }

                clickedWindow = clickedWindow->childAt( event.position() - clickedWindow->dimensions().topLeft );
            }
        }
    }

    void Application::refresh( bool /*force*/ ){
        wrefresh( window() );
    }

    void Application::onRootResized(){
        struct winsize winSize;
        ioctl( STDOUT_FILENO, TIOCGWINSZ, &winSize );
        bool sizeChanged = ( dimensions().size.width != winSize.ws_col || dimensions().size.height != winSize.ws_row );

        if( sizeChanged ){
            Rectangle rect;
            rect.size.width = winSize.ws_col;
            rect.size.height = winSize.ws_row;
            ::clear();
            ::endwin();
            setDimensions( rect );
            ::wrefresh( window() );
        }
    }

    void Application::run( ){
        STDINFileEventHandler stdinHandler( this );
        _reactor.addFileEventHandler( &stdinHandler );

        Timer timer(std::chrono::milliseconds(10));
        timer.signals.timeout.connect( std::bind( &Application::processDelayedActions, this ) );
        reactor().addFileEventHandler( &timer );
        
        while( exit() == false ){ 
            _reactor.poll(); 
            //std::this_thread::sleep_for( std::chrono::milliseconds(10) );
        }
    }

    void Application::addDelayedAction( std::chrono::milliseconds delay, std::function<void()> action ){
        _delayedActions.emplace_back( DelayedAction{std::chrono::high_resolution_clock::now() + delay, action} );
    }

    void Application::processDelayedActions(){
        auto now = std::chrono::high_resolution_clock::now();
        for( size_t i = 0; i < _delayedActions.size(); ++i ){
            if( _delayedActions[i].triggerTime < now ){
                _delayedActions[i].action();
                _delayedActions.erase( _delayedActions.begin() + i );
            }
        }
    }
}

