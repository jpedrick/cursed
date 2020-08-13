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
#include "Color.hpp"
#include "Label.hpp"
#include "LineEdit.hpp"
#include "Spacer.hpp"
#include "ScrolledListView.hpp"
#include "IListModel.hpp"
#include "ListModelIndex.hpp"

#include <vector>
#include <fstream>

class ListModel : public cursed::IListModel{
public:
    int64_t rowCount() const{
        return _rows.size();
    }

    std::vector<int> displayRoles() const{
        return std::vector<int>{ 0, 1 };
    }

    const std::string& roleName( int role ) const {
        return _roleNames[ role ];
    }

    const std::string& text( const cursed::ListModelIndex& ix, int displayRole ) const{
        return _rows[ix.rowIndex()].data[ displayRole ];
    }

    unsigned long attributes(  const cursed::ListModelIndex& ix, int unused(displayRole) ) const{
        return _rows[ix.rowIndex()].attributes;
    }

    struct Item{
        std::vector<std::string> data;
        unsigned long attributes;
    };
    void insertItem( int ix, const Item& item ){
        _rows.insert(_rows.begin() + ix, item);
        IListModel::signals.rowsInserted( ix, 1 );
    }
private:
    std::vector<Item> _rows;
    std::vector<std::string> _roleNames = { "field 1|", "field 2|" };
};

int main( int unused(argc), char* unused(argv)[] ){
    using namespace cursed;
    cursed::Log::setOutputStream( new std::ofstream("test_example_line_edit_log.txt") );
    cursed_out( "running application: " << cprint(argv[0]) );

    ListModel model;
    Label* label = nullptr;
    LineEdit* lineEdit1 = nullptr;
    LineEdit* lineEdit2 = nullptr;
    ScrolledListView* listView = nullptr;
    Application app{ argv[0], 
        Direction::Vertical, {
            { 0, label = new Label{ "Enter some text: " } }, 
                cursed::LayoutObject{ 1, new Window{ Direction::Horizontal, "line-edits", {
                    { 1, new Label{ "field 1| " } },
                    { 10, lineEdit1 = new LineEdit{ Direction::Horizontal, "lineEdit1" } },
                    { 1, new Label{ "field 2| " } },
                    { 10, lineEdit2 = new LineEdit{ Direction::Horizontal, "lineEdit2" } },
                } } },
            { 10, listView = new ScrolledListView{ &model } }
        }
    };

    app.setFocus( lineEdit2 );

    {
        SizeLimits limits = label->sizeLimits();
        limits.minimum.height = 1;
        label->setSizeLimits( limits );
    }

    const cursed::Color darkblue  = {   0,    0 ,200 };
    const cursed::Color bluegrey = { 200,  200, 300 };

    const cursed::Color bluewhite= { 900,  900,1000 };
    const cursed::Color nearblack= {  50,   50,  50 };

    const cursed::Color darkGreen = { 0, 300, 100 };
    const cursed::Color lightGreen = { 400, 700, 400 };
    cursed::ColorPair l1ItemColor{ lightGreen, darkGreen };
    cursed::ColorPair l2ItemColor{ darkGreen, lightGreen };

    cursed::ColorPair labelColor{ bluegrey, darkblue };
    label->setColor( labelColor );

    lineEdit1->lineEditSignals.returnPressed.connect([&]{
        model.insertItem(0, ListModel::Item{ { lineEdit1->text(), lineEdit2->text() }, l1ItemColor } );
    });
    lineEdit2->lineEditSignals.returnPressed.connect([&]{
        model.insertItem(0, ListModel::Item{ { lineEdit1->text(), lineEdit2->text() }, l1ItemColor } );
    });

    app.refresh();

    app.start();
    app.run( );

    return 0;
}
