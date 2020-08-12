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

#include "MouseEventWindow.hpp"
#include "Signal.hpp"
#include "Color.hpp"

#include <functional>

namespace cursed{

struct Int64ValueRange{ 
    int64_t min = 0; 
    int64_t max = 100; 

    static double getPercent( int64_t value, const Int64ValueRange& range );
    static int64_t getRange( const Int64ValueRange& range );

    int64_t clip( int64_t v ) const {
        return std::min( max, std::max( min, v ) );
    }
};

struct ProgressBarConstructorArgs{
    int64_t value = 0;
    Int64ValueRange range = {0,100};
};

class ProgressBar : public MouseEventWindow{
public:
    static constexpr char defaultHorizonalIndicator[] = "║";
    static constexpr char defaultVerticalIndicator[] = "═";

    ProgressBar( Direction layout, 
                 const ProgressBarConstructorArgs& builderArgs = {},
                 const std::string& name = "" );

    struct visualProperties{

    };

    typedef int64_t NewValue;
    typedef int64_t OldValue;
    typedef Signal< NewValue, OldValue > ValueChanged;

    enum class Area{
        Value,
        Remaining
    };
    typedef Signal< Area, MouseButton > Clicked;

    struct {
        ValueChanged valueChanged; // pressed and released mouse 0 while cursor in window area
        Clicked clicked;
    } progressBarSignals;

    int64_t setValue( int64_t v );

    int64_t value() const{ return _value; }

    void setValueRange( const Int64ValueRange& valueRange );
    const Int64ValueRange& setValueRange( );

    int64_t adjustValue( int64_t change );

    void update( bool force = true );

    void draw( bool fullRefresh ) override;

    void setColors( unsigned long valueColor, unsigned long remainingColor ){
        _visualProperties.valueColor = valueColor;
        _visualProperties.remainingColor = remainingColor;
    }

private:
    struct VisualProperties{
        unsigned long valueColor = 0;
        unsigned long remainingColor = 0;
        unsigned long attributes = 0;
    } _visualProperties;

    int64_t _value;
    Int64ValueRange _valueRange;
    Direction _layoutDirection;
    bool _show = true;
    Rectangle _filledArea;
    Rectangle _unfilledArea;
};


}
