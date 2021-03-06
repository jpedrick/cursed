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


int main( int __attribute__((unused))argc, char* __attribute__((unused))argv[] ){
    using namespace cursed;
    ::setlocale(LC_ALL, "");

    Button* b1 = nullptr;
    Button* b2 = nullptr;
    Button* b3 = nullptr;
    Button* b4 = nullptr;
    Label* label = nullptr;
    ScrollBar* scroller = nullptr;
    Application app{ Direction::Vertical, {
        { 0, label = new Label{ "some text for a label" } }, 
        { 0, scroller = new ScrollBar{ Direction::Horizontal } }, 
        { 1, new Window{ 
            Direction::Horizontal, "button-window",
            { 
                {1, new Window{ Direction::Vertical, "leftvpanel", { 
                        { 1, b1 = new Button{ "b1", Direction::Horizontal, "b1" } },
                        { 1, b2 = new Button{ "b2", Direction::Horizontal, "b2" } }
                    } } 
                },
                {1, new Window{ Direction::Vertical, "leftvpanel", { 
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
        SizeLimits limits = scroller->sizeLimits();
        limits.minimum.height = 1;
        scroller->setSizeLimits( limits );
        scroller->setMaxValue(20);
        scroller->setValue(0);
        scroller->setButtonIncrement(4);
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
    const cursed::Color green  = {   0, 1000 ,   0 };
    const cursed::Color blue   = {   0,    0 ,1000 };
    const cursed::Color darkblue  = {   0,    0 ,200 };
    const cursed::Color yellow = {  600, 600 ,   0 };

    const cursed::Color bluegrey = { 200,  200, 300 };
    const cursed::Color bluewhite= { 900,  900,1000 };
    const cursed::Color nearblack= {  50,   50,  50 };

    cursed::ColorPair labelColor{ bluegrey, darkblue };
    label->setColor( labelColor );

    cursed::ColorPair scrollButtonsNormal{ bluewhite, yellow };
    cursed::ColorPair scrollButtonsPressed{ yellow, bluewhite };
    scroller->setButtonColors( scrollButtonsNormal, scrollButtonsPressed );

    cursed::ColorPair indicatorNormal{ red, yellow };
    cursed::ColorPair indicatorPressed{ blue, green };
    scroller->setIndicatorColors( indicatorNormal, indicatorPressed );

    cursed::ColorPair b1Normal{ nearblack, bluewhite };
    cursed::ColorPair b1Pressed{ bluewhite, nearblack };
    b1->setColors(b1Normal, b1Pressed);

    cursed::ColorPair b2Normal{ red, yellow };
    cursed::ColorPair b2Pressed{ yellow, red };
    b2->setColors(b2Normal, b2Pressed);

    cursed::ColorPair b3Normal{ blue, green };
    cursed::ColorPair b3Pressed{ green, blue };
    b3->setColors(b3Normal, b3Pressed);

    cursed::ColorPair b4Normal{ yellow , blue };
    cursed::ColorPair b4Pressed{ blue, yellow };
    b4->setColors(b4Normal, b4Pressed);

    b1->signals.clicked.connect([label]( ){ label->setText( "b1 clicked " ); });
    b1->signals.doubleClicked.connect([label]( ){ label->setText( "b1 double-clicked " ); });
    b1->signals.tripleClicked.connect([label]( ){ label->setText( "b1 triple-clicked " ); });

    b1->signals.pressed.connect([&]{ label->setText( "b1 pressed" ); app.refresh(); });
    b1->signals.released.connect([&]{ label->setText( "b1 released" ); app.refresh(); });

    b2->signals.clicked.connect([&]( ){ label->setText( "b2 clicked " ); });
    b2->signals.pressed.connect([&]{ label->setText( "b2 pressed" ); });
    b2->signals.released.connect([&]{ label->setText( "b2 released" ); });

    b3->signals.clicked.connect([&]( ){ label->setText( "b3 clicked " ); });
    b3->signals.pressed.connect([&]{ label->setText( "b3 pressed" ); });
    b3->signals.released.connect([&]{ label->setText( "b3 released" ); });

    b4->signals.clicked.connect([&]( ){ label->setText( "b4 clicked " ); });
    b4->signals.pressed.connect([&]{ label->setText( "b4 pressed" ); });
    b4->signals.released.connect([&]{ label->setText( "b4 released" ); });

    scroller->signals.valueChanged.connect([&]( ScrollBar::NewValue newVal, ScrollBar::OldValue oldVal ){
        std::stringstream ss;
        ss << " scrollbar value changed " << cprint(newVal) << cprint(oldVal) << cprint(scroller->maxValue());
        label->setText( ss.str() );
    });

    app.refresh();

    app.start();
    app.run( );

    return 0;
}
