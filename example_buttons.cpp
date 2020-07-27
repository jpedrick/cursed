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
#include "Color.hpp"
#include "Button.hpp"
#include "Label.hpp"
#include "ScrollBar.hpp"
#include <fstream>

int main( int __attribute__((unused))argc, char* __attribute__((unused))argv[] ){
    using namespace cursed;
    ::setlocale(LC_ALL, "");
    cursed::Log::setOutputStream( new std::ofstream("test_example_buttons.txt") );
    cursed_out( "running application: " << cprint(argv[0]) );

    Button* b1 = nullptr;
    Button* b2 = nullptr;
    Button* b3 = nullptr;
    Button* b4 = nullptr;
    Label* label = nullptr;
    ScrollBar* hscroller = nullptr;
    ScrollBar* vscroller = nullptr;
    Application app{ Direction::Vertical, {
        { 0, label = new Label{ "some text for a label" } }, 
        { 1, hscroller = new ScrollBar{ Direction::Horizontal, "horizontal-scroll" } }, 
        { 10, new Window{ 
            Direction::Horizontal, "button-window",
            { 
                {1, vscroller = new ScrollBar{ Direction::Vertical, "vertical-scroll" } }, 
                {5, new Window{ Direction::Vertical, "leftvpanel", { 
                        { 1, b1 = new Button{ "b1", Direction::Horizontal, "b1" } },
                        { 1, b2 = new Button{ "b2", Direction::Horizontal, "b2" } }
                    } } 
                },
                {5, new Window{ Direction::Vertical, "leftvpanel", { 
                        { 1, b3 = new Button{ "b3", Direction::Horizontal, "b3" } },
                        { 1, b4 = new Button{ "b4", Direction::Horizontal, "b4" } }
                    } } 
                }
            }
        }}
    }};

    {
        SizeLimits limits = label->sizeLimits();
        limits.minimum.height = 1;
        label->setSizeLimits( limits );
    }

    {
        SizeLimits limits = hscroller->sizeLimits();
        limits.minimum.height = 1;
        hscroller->setSizeLimits( limits );
        hscroller->setMaxValue(20);
        hscroller->setValue(0);
        hscroller->setButtonIncrement(4);
    }

    {
        SizeLimits limits = hscroller->sizeLimits();
        limits.minimum.width = 1;
        vscroller->setSizeLimits( limits );
        vscroller->setMaxValue(200);
        vscroller->setValue(0);
        vscroller->setButtonIncrement(20);
    }

    {
        SizeLimits limits = b1->sizeLimits();
        limits.maximum.height = 5;
        limits.maximum.width = 15;
        b1->setSizeLimits( limits );
    }
    b2->setSizeLimits( SizeLimits::unlimited() );
    b3->setSizeLimits( SizeLimits::unlimited() );
    b4->setSizeLimits( SizeLimits::unlimited() );

    const cursed::Color red    = {1000,    0 ,   0 };
    const cursed::Color mutedGreen  = {   250, 500, 300 };
    const cursed::Color darkGreen  = {   0, 500 ,   0 };
    const cursed::Color blue   = {   0,    0 ,1000 };
    const cursed::Color darkblue  = {   0,    0 ,200 };
    const cursed::Color yellow = {  600, 600 ,   0 };

    const cursed::Color bluegrey = { 200,  200, 300 };
    const cursed::Color bluewhite= { 900,  900,1000 };
    const cursed::Color nearblack= {  50,   50,  50 };

    cursed::ColorPair labelColor{ bluegrey, darkblue };
    label->setColor( labelColor );

    cursed::ColorPair scrollButtonsNormal{ mutedGreen, darkGreen };
    cursed::ColorPair scrollButtonsPressed{ yellow, bluewhite };
    hscroller->setButtonColors( scrollButtonsNormal, scrollButtonsPressed );
    vscroller->setButtonColors( scrollButtonsNormal, scrollButtonsPressed );

    cursed::ColorPair indicatorNormal{ red, yellow };
    cursed::ColorPair indicatorPressed{ blue, mutedGreen };
    hscroller->setIndicatorColors( indicatorNormal, indicatorPressed );
    vscroller->setIndicatorColors( indicatorNormal, indicatorPressed );

    cursed::ColorPair b1Normal{ nearblack, bluewhite };
    cursed::ColorPair b1Pressed{ bluewhite, nearblack };
    b1->setColors(b1Normal, b1Pressed);

    cursed::ColorPair b2Normal{ red, yellow };
    cursed::ColorPair b2Pressed{ yellow, red };
    b2->setColors(b2Normal, b2Pressed);

    cursed::ColorPair b3Normal{ blue, mutedGreen };
    cursed::ColorPair b3Pressed{ mutedGreen, blue };
    b3->setColors(b3Normal, b3Pressed);

    cursed::ColorPair b4Normal{ yellow , blue };
    cursed::ColorPair b4Pressed{ blue, yellow };
    b4->setColors(b4Normal, b4Pressed);

    b1->signals.clicked.connect([label]( int mouseButton ){ label->setText( "b1 clicked with mouseButton: " + std::to_string(mouseButton) ); });
    b1->signals.doubleClicked.connect([label]( int mouseButton ){ label->setText( "b1 double-clicked with mouseButton: " + std::to_string(mouseButton) ); });
    b1->signals.tripleClicked.connect([label]( int mouseButton ){ label->setText( "b1 triple-clicked with mouseButton: " + std::to_string(mouseButton) ); });

    b1->signals.pressed.connect([&]( int mouseButton ){ label->setText( "pressed b1 with mouseButton " + std::to_string(mouseButton) ); app.refresh(); });
    b1->signals.released.connect([&]( int mouseButton ){ label->setText( "released b1" ); app.refresh(); });

    b2->signals.clicked.connect([&]( int mouseButton ){ label->setText( "b2 clicked " ); });
    b2->signals.pressed.connect([&]( int mouseButton ) { label->setText( "b2 pressed" ); });
    b2->signals.released.connect([&]( int mouseButton ){ label->setText( "b2 released" ); });

    b3->signals.clicked.connect([&]( int mouseButton ){ label->setText( "b3 clicked " ); });
    b3->signals.pressed.connect([&]( int mouseButton ){ label->setText( "b3 pressed" ); });
    b3->signals.released.connect([&]( int mouseButton ){ label->setText( "b3 released" ); });

    b4->signals.clicked.connect([&]( int mouseButton ){ label->setText( "b4 clicked " ); });
    b4->signals.pressed.connect([&]( int mouseButton ){ label->setText( "b4 pressed" ); });
    b4->signals.released.connect([&]( int mouseButton ){ label->setText( "b4 released" ); });

    hscroller->signals.valueChanged.connect([&]( ScrollBar::NewValue newVal, ScrollBar::OldValue oldVal ){
        std::stringstream ss;
        ss << "horizonal scrollbar value changed " << cprint(newVal) << cprint(oldVal) << cprint(hscroller->maxValue());
        label->setText( ss.str() );
    });

    vscroller->signals.valueChanged.connect([&]( ScrollBar::NewValue newVal, ScrollBar::OldValue oldVal ){
        std::stringstream ss;
        ss << "vertical scrollbar value changed " << cprint(newVal) << cprint(oldVal) << cprint(hscroller->maxValue());
        label->setText( ss.str() );
    });

    app.refresh();

    app.start();
    app.run( );

    return 0;
}
