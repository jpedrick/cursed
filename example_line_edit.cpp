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

class ListModel : public cursed::IListModel{
public:
    int64_t rowCount() const{
        return _rows.size();
    }

    std::vector<int> displayRoles() const{
        return {1};
    }

    const std::string& roleName( int role ) const {
        return _roleNames[ role ];
    }

    const std::string& text( const cursed::ListModelIndex& ix, int unused(displayRole) ) const{
        return _rows[ix.rowIndex()].data;
    }

    unsigned long attributes(  const cursed::ListModelIndex& unused(ix), int unused(displayRole) ) const{
        return _rows[ix.rowIndex()].attributes;
    }

    struct Item{
        std::string data;
        unsigned long attributes;
    };
    void insertItem( int ix, const Item& item ){
        _rows.insert(_rows.begin() + ix, item);
        IListModel::signals.rowsInserted( ix, 1 );
    }
private:
    std::vector<Item> _rows;
    std::vector<std::string> _roleNames = { "data" };
};

int main( int __attribute__((unused))argc, char* __attribute__((unused))argv[] ){
    using namespace cursed;

    ListModel model;
    Label* label = nullptr;
    LineEdit* lineEdit = nullptr;
    LineEdit* lineEdit2 = nullptr;
    ScrolledListView* listView = nullptr;
    Application app{ Direction::Vertical, {
        { 0, label = new Label{ "some text for a label" } }, 
        { 0, lineEdit = new LineEdit{ Direction::Horizontal } },
        { 0, lineEdit2 = new LineEdit{ Direction::Horizontal } },
        { 1, listView = new ScrolledListView{ &model } }
    }};

    app.setFocus( lineEdit );

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

    lineEdit->signals.returnPressed.connect([&]{
        label->setText( "lineEdit return pressed: " + lineEdit->text() );
        model.insertItem(0, ListModel::Item{ lineEdit->text(), l1ItemColor } );
    });
    lineEdit2->signals.returnPressed.connect([&]{
        label->setText( "lineEdit2 return pressed: " + lineEdit2->text() );
    });

    app.refresh();

    app.start();
    app.run( );

    return 0;
}
