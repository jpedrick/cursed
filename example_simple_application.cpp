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
#include "Window.hpp"
#include "Direction.hpp"
#include "Draw.hpp"
#include "Signal.hpp"
#include "Timer.hpp"
#include "IListModel.hpp"
#include "ListModelIndex.hpp"
#include "ListView.hpp"
#include "Color.hpp"
#include "Button.hpp"
#include "Macros.hpp"

#include <fstream>
#include <thread>
#include <chrono>

#include <ctime>


class TimeWindow : public cursed::Window {
public:
    TimeWindow() = delete;
    TimeWindow( const TimeWindow& ) = delete;
    TimeWindow( TimeWindow&& ) = delete;

    template< typename... Args >
    TimeWindow( Args&&... args ) : 
        Window( std::forward<Args>(args)... )
    { 
        setSizeLimits( cursed::SizeLimits{ {1,1}, {std::numeric_limits<int>::max(), 1} } );
    }

    void onTimeout(){
        refresh( false );
    }
    void draw( bool ){
        time_t now;
        ::time( &now );
        struct tm* timeinfo = ::localtime( &now );
        std::string timeStr = (name() + "/" + asctime(timeinfo));
        cursed_out( "time is: " << echo_stream(timeStr) );
        cursed::Draw::text( this, {0,0}, timeStr );
    }
};

class GridWindow : public cursed::Window {
public:
    template< typename... Args >
    GridWindow( int gridSize, Args&&... args ) : 
        Window( std::forward<Args>(args)... ), 
        _gridSize(gridSize) 
    { }

    void draw( bool fullRefresh ){
        if( fullRefresh ){
            cursed::Rectangle dim = dimensions();

            cursed::Draw::AttributeGuard ag{ this, A_ALTCHARSET };
            for( int i = 0; i <= dim.size.width; i += _gridSize ){
                cursed::Draw::line( this, cursed::Direction::Vertical, i, ACS_VLINE );
            }
            for( int i = 0; i <= dim.size.height; i += _gridSize ){
                cursed::Draw::line( this, cursed::Direction::Horizontal, i, ACS_HLINE );
            }
            for( int i = 0; i <= dim.size.width; i += _gridSize ){
                for( int j = 0; j <= dim.size.height; j += _gridSize ){
                    if( i == 0 && j == 0 ){
                        cursed::Draw::character( this, {i,j}, ACS_ULCORNER ); // Upper left
                    } else if( i == _gridSize && j == dim.size.height ){
                        cursed::Draw::character( this, {i,j}, ACS_LRCORNER ); // Lower right
                    } else if( i == 0 && j == dim.size.height ){
                        cursed::Draw::character( this, {i,j}, ACS_LLCORNER ); // Lower left
                    } else if( i == dim.size.width && j == 0 ){
                        cursed::Draw::character( this, {i,j}, ACS_URCORNER ); // Upper Right
                    } else if( i == 0 ){
                        cursed::Draw::character( this, {i,j}, ACS_LTEE );//Left T
                    } else if( j == 0 ){
                        cursed::Draw::character( this, {i,j}, ACS_TTEE );//Top T
                    } else if( i == dim.size.width ){
                        cursed::Draw::character( this, {i,j}, ACS_RTEE );//Right T
                    } else if( j == dim.size.height ){
                        cursed::Draw::character( this, {i,j}, ACS_BTEE );// Bottom T
                    } else {
                        cursed::Draw::character( this, {i,j}, ACS_PLUS );
                    }
                }
            }
        }
    }

private:
    int _gridSize;
};

class ExampleListModel : public cursed::IListModel{
public:
    ExampleListModel() : 
        _rows{ 
            RowItem{ "I", "am", "Sparta!", "HHHH", "YYYY" },
            RowItem{ "Et", "tu", "Brute?", "XXXX", "ZZZZ" },
            RowItem{ "🌞", "🐅🐅🐅🐅🐅🐅", "🐦🐦🐦🐦🐦🐦🐦🐦", "🦖🦖🦖🦖🦖🦖🦖🦖", "🤡" }
        },
        _roleNames{ "first", "second", "third", "4th", "5th" }
    {
    }
    ~ExampleListModel(){}
    int64_t rowCount() const{
        return _rows.size();
    }
    std::vector<int> displayRoles() const{ return { 0, 1, 2, 3, 4 }; }

    const std::string _badRole = "bad-role";
    const std::string& roleName( int role ) const{
        switch( role ){
        case 0:
            return std::get<0>(_roleNames);
        case 1:
            return std::get<1>(_roleNames);
        case 2:
            return std::get<2>(_roleNames);
        case 3:
            return std::get<3>(_roleNames);
        case 4:
            return std::get<4>(_roleNames);
        }
        return _badRole;
    }

    const std::string _badIndex = "bad-index";
    const std::string& text( const cursed::ListModelIndex& ix, int role ) const{
        if( ix.valid() ){
            switch( role ){
            case 0:
                return std::get<0>(_rows[ix.rowIndex()]);
            case 1:
                return std::get<1>(_rows[ix.rowIndex()]);
            case 2:
                return std::get<2>(_rows[ix.rowIndex()]);
            case 3:
                return std::get<3>(_rows[ix.rowIndex()]);
            case 4:
                return std::get<4>(_rows[ix.rowIndex()]);
            }
        }
        return _badIndex;
    }

    void setText( const std::string& val, const cursed::ListModelIndex& ix, int role ) {
        if( ix.valid() ){
            switch( role ){
            case 0:
                std::get<0>(_rows[ix.rowIndex()]) = val;
                break;
            case 1:
                std::get<1>(_rows[ix.rowIndex()]) = val;
                break;
            case 2:
                std::get<2>(_rows[ix.rowIndex()]) = val;
                break;
            case 3:
                std::get<3>(_rows[ix.rowIndex()]) = val;
                break;
            case 4:
                std::get<4>(_rows[ix.rowIndex()]) = val;
                break;
            default: break;
            } 
            signals.dataChanged( ix, ix );
        }
    }

    unsigned long attributes(  const cursed::ListModelIndex& unused(ix), int unused(role) ) const{
        return 0;
    }
private:
    typedef std::tuple< std::string, std::string, std::string, std::string, std::string > RowItem;
    typedef std::vector< RowItem > RowItems;

    RowItems _rows;

    RowItem _roleNames;
};

int main( int unused(argc), char* unused(argv[]) ){
    using namespace cursed;

    cursed::Log::_outputStream = new std::ofstream("test_app_log.txt");
    cursed_out( "running application: " << cprint(argv[0]) );

    IWindow* gridwin = nullptr;
    ListView* listView = nullptr;
    TimeWindow* timewin1 = nullptr;
    TimeWindow* timewin2 = nullptr;
    TimeWindow* timewin3 = nullptr;
    Button* button1 = nullptr;


    ExampleListModel listModel;

    IWindow* rightPanel =  new Window{ Direction::Vertical, "rightPanel",
        { 
            { 1, gridwin = new GridWindow{ 4, Direction::Horizontal, "gridwin" } },
            { 2, listView = new ListView{ &listModel } }
        }
    }; 

    IWindow* leftPanel = new Window{ Direction::Vertical, "leftvpanel", { 
                    cursed::LayoutObject{ 1, timewin1 = new TimeWindow{ Direction::Horizontal, "time1" } },
                    cursed::LayoutObject{ 1, timewin2 = new TimeWindow{ Direction::Horizontal, "time2" } },
                    cursed::LayoutObject{ 1, timewin3 = new TimeWindow{ Direction::Horizontal, "time3" } },
                    cursed::LayoutObject{ 1, button1 = new Button{ "Push Me!", Direction::Horizontal, "button1" } },
                } };

    cursed::Application app{ Direction::Horizontal, 
        { 
            {1, leftPanel }
           ,{1, rightPanel } 
        }
    };

    cursed_out( "Application: " << cprint(app.dimensions().size.width) );

    const cursed::Color red   = {1000,    0 ,   0 };
    const cursed::Color green = {   0, 1000 ,   0 };
    const cursed::Color blue  = {   0,    0 ,1000 };

    const cursed::Color bluegrey = { 200,  200, 300 };
    const cursed::Color bluewhite= { 900,  900,1000 };
    const cursed::Color black    = {   0,    0,   0 };

    cursed::ColorPair coolGrey(bluewhite, bluegrey);
    cursed::ColorPair greyCool(bluegrey, bluewhite);
    listView->setRoleStripingColors( coolGrey, greyCool );

    cursed::ColorPair buttonNormal{ black, bluewhite };
    cursed::ColorPair buttonPressed{ bluewhite, black };
    button1->setColors( buttonNormal, buttonPressed );

    app.refresh();

    Timer timer(std::chrono::seconds(1));
    app.reactor().addFileEventHandler( &timer );

    timer.signals.timeout.connect( [&]{ 
        timewin1->onTimeout(); 
        timewin2->onTimeout(); 
    } );

    auto ix = listModel.index(1);
    int count = 0;
    timer.signals.timeout.connect( [&]{ 
        timewin3->onTimeout(); 
        listModel.setText(std::to_string(count++),ix,0);
    } );
    timer.signals.timeout.connect( [&]{ 
        app.refresh();
    } );

    app.start();
    app.run( );
        
    return 0;
}

