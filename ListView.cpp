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
#include "ListView.hpp"
#include "IListModel.hpp"
#include "IListItemDelegate.hpp"
#include "DefaultListItemDelegate.cpp"

#include "ListModelIndex.hpp"
#include "Draw.hpp"

namespace cursed{

static DefaultListItemDelegate defaultListItemDelegate;

void ListView::setDataModel( IListModel* m ){
    reset();

    _model = m;
    connections.dataChanged = _model->signals.dataChanged.connect( 
        [this]( const ListModelIndex& begin, const ListModelIndex& end ){ 
            this->onDataChanged(begin, end); 
        });

    connections.rowsInserted = _model->signals.rowsInserted.connect( 
        [this]( int64_t begin, int64_t count ){ 
            this->onRowsInserted(begin, count ); 
        });

    connections.rowsRemoved = _model->signals.rowsRemoved.connect( 
        [this]( int64_t begin, int64_t count ){ 
            this->onRowsRemoved(begin, count ); 
        });
    _displayRoles = _model->displayRoles();
}

void ListView::onRowsInserted( int64_t begin, int64_t count ){
    if( begin < firstVisibleRow() ){
        _viewport.y() += (int)count;
    }

    refresh(true);
}

void ListView::onRowsRemoved( int64_t unused(begin), int64_t unused(count) ){
    if( begin < firstVisibleRow() ){
        _viewport.y() = std::max(0L, _viewport.y() - count );
    }

    refresh(true);
}

void ListView::reset(){
    if( _model ){
        connections.dataChanged.disconnect();
        connections.rowsInserted.disconnect();
        connections.rowsRemoved.disconnect();
    }
}

void ListView::onDataChanged( const ListModelIndex& begin, const ListModelIndex& end ){
    int64_t firstVisible = firstVisibleRow();
    int64_t lastVisible = firstVisible + dimensions().size.height;

    int64_t first = std::max( begin.rowIndex(), firstVisible );
    int64_t last = std::min( end.rowIndex(), lastVisible );

    for( int64_t refreshRow = first; refreshRow <= last; ++refreshRow ){
        int rowIndex = refreshRow;
        const IListItemDelegate* thisRowDelegate = rowDelegate(rowIndex);
        const IListItemDelegate* itemDelegate = thisRowDelegate ? thisRowDelegate : delegates._default; 
        bool evenRole = true;
        int roleIx = 0;
        for( int role : _displayRoles ){
            const IListItemDelegate* thisRoleDelegate = roleDelegate(role);
            itemDelegate = thisRoleDelegate ? thisRoleDelegate : itemDelegate; 
            const IListItemDelegate* thisCellDelegate = cellDelegate({rowIndex, role});
            itemDelegate = thisCellDelegate ? thisCellDelegate : itemDelegate; 

            ListModelIndex index{ _model, rowIndex }; 
            Rectangle allocation = indexGeometry( index, role );

            unsigned long cellAttributes = index.attributes( role );
            unsigned long color = evenRole ? visualProperties.evenRoleColors : visualProperties.oddRoleColors;
            Draw::AttributeGuard ag{ this, cellAttributes | color };

            itemDelegate->draw(this, index, role, allocation );
            evenRole = !evenRole;
            ++roleIx;
        }
    }
}

void ListView::adjustViewportTopLeft( int y, int x ){
    bool needsRefresh = y != 0 || x != 0;
    _viewport.x() += x;
    _viewport.y() += y; 
    if( needsRefresh ) this->refresh(true);
}
void ListView::setViewportTopLeft( int y, int x ){
    bool needsRefresh = y != _viewport.y() || x != _viewport.x();

    _viewport.y() = y;
    _viewport.x() = x;

    if( needsRefresh ) this->refresh(true);
}

int ListView::visibleRows() const{
    return this->dimensions().size.height;
}

void ListView::setDefaultDelegate( IListItemDelegate* delegate ){
    delegates._default = delegate;
}

void ListView::setRoleDelegate( int role, IListItemDelegate* delegate ){
    delegates.role[role] = delegate;
}

void ListView::setRowDelegate( int row, IListItemDelegate* delegate ){
    delegates.role[row] = delegate;
}

void ListView::setCellDelegate( ListViewIndex ix, IListItemDelegate* delegate ){
    delegates.cell[ix] = delegate;
}

const IListItemDelegate* ListView::roleDelegate( int role ) const{
    auto delegate = delegates.role.find( role );
    if( delegate != delegates.role.end() ) return delegate->second;
    return nullptr;
}

const IListItemDelegate* ListView::rowDelegate( int row ) const{
    auto delegate = delegates.row.find( row );
    if( delegate != delegates.row.end() ) return delegate->second;
    return nullptr;
}

const IListItemDelegate* ListView::cellDelegate( ListViewIndex ix ) const{
    auto delegate = delegates.cell.find( ix );
    if( delegate != delegates.cell.end() ) return delegate->second;
    return nullptr;
}

int64_t ListView::firstVisibleRow() const{
    return std::max( 0L, std::min( (long)_viewport.y(), std::max(0L,_model->rowCount() - visibleRows()) ) );
}

Rectangle ListView::indexGeometry( const ListModelIndex& ix, int role ) const{
    Rectangle rv;
    RoleGeometry roleGeom = roleGeometry( role );
    int64_t firstVisible = firstVisibleRow();
    int64_t rowY = ix.rowIndex() - firstVisible; //< rowY may be off the screen.

    rv.topLeft.x() = roleGeom.startX;
    rv.topLeft.y() = rowY;

    rv.size.height = 1;
    rv.size.width  = roleGeom.width;

    return rv;
}

ListView::RoleGeometry ListView::roleGeometry( int role ) const{
    return _roleGeometry.find(role)->second;
}

void ListView::draw( bool fullRefresh ){
    if( fullRefresh ){
        int columnCount = _displayRoles.size();
        {
            int columnWidth = this->dimensions().size.width / columnCount;
            int curx = 0;
            for( int role : _displayRoles ){
                RoleGeometry geom;
                geom.startX = curx;
                geom.width = columnWidth;
                curx += columnWidth;
                _roleGeometry[role] = geom;
            }
        }

        if( _viewport.y() < 0 ) _viewport.y() = 0;
        int startRow = std::min( (long)_viewport.y(), std::max(0L,_model->rowCount() - visibleRows()) );
        if( !delegates._default ) delegates._default = &defaultListItemDelegate;
        for( int i = 0; i < std::min( (int64_t)visibleRows(), _model->rowCount()); ++i ){
            int rowIndex = startRow + i;
            const IListItemDelegate* thisRowDelegate = rowDelegate(rowIndex);
            const IListItemDelegate* itemDelegate = thisRowDelegate ? thisRowDelegate : delegates._default; 
            bool evenRole = true;
            int roleIx = 0;
            for( int role : _displayRoles ){
                const IListItemDelegate* thisRoleDelegate = roleDelegate(role);
                itemDelegate = thisRoleDelegate ? thisRoleDelegate : itemDelegate; 
                const IListItemDelegate* thisCellDelegate = cellDelegate({rowIndex, role});
                itemDelegate = thisCellDelegate ? thisCellDelegate : itemDelegate; 

                ListModelIndex index{ _model, rowIndex }; 
                Rectangle allocation = indexGeometry( index, role );

                unsigned long cellAttributes = index.attributes( role );
                unsigned long color = evenRole ? visualProperties.evenRoleColors : visualProperties.oddRoleColors;
                Draw::AttributeGuard ag{ this, cellAttributes | color };
                
                itemDelegate->draw(this, index, role, allocation );
                evenRole = !evenRole;
                ++roleIx;
            }
        }
    }
}

void ListView::setRoleStripingColors( unsigned long evenRole, unsigned long oddRole ){
    visualProperties.evenRoleColors = evenRole;
    visualProperties.oddRoleColors = oddRole;
}

}
