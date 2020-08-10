
#include "ProgressBar.hpp"
#include "Draw.hpp"

namespace cursed{

ProgressBar::ProgressBar( Direction layout, 
             const ProgressBarConstructorArgs& builderArgs,
             const std::string& name ) : MouseEventWindow{ layout, name }
    , _valueRange( builderArgs.range )
    , _colors( builderArgs.colors )
    , _value( builderArgs.value )
{
    signals.clicked.connect( [&]( MouseButton button, Point relativePos ){
        if( _filledArea.contains( relativePos ) ){
            //signals.progressAreaClicked.emit( Area::Value, button );
        }
        else if( _unfilledArea.contains( relativePos ) ){
            //signals.remainingAreaClicked.emit( Area::Remaining, button );
        }
    }, { MouseButton::Left, {} } );
}

double Int64ValueRange::getPercent( int64_t value, const Int64ValueRange& valueRange ){
    const auto range = Int64ValueRange::getRange( valueRange );
    if( range != 0 ){
        return std::max( 0.0, std::min(1.0, double(value - valueRange.min)  / range) );
    }

    return 1;
}

int64_t Int64ValueRange::getRange( const Int64ValueRange& range ){
    return range.max - range.min;
}

void ProgressBar::setValue( int64_t v ){
    progressBarSignals.valueChanged.emitAfter( [this, v]{
        this->_value = v;
        draw( true );
    }, v, _value );
}

void ProgressBar::setValueRange( const Int64ValueRange& valueRange ){
    _valueRange = valueRange;
}

void ProgressBar::draw( bool fullRefresh ){
    if( fullRefresh ){
        Rectangle dim = dimensions();

        int filledSize = dim.size.getDimension( _layoutDirection ) * Int64ValueRange::getPercent( _value, _valueRange );
        int unfilledSize = dim.size.getDimension( _layoutDirection ) - filledSize;
        
        //unsigned long color = _pressed ? visualProperties.pressedColor : visualProperties.normalColor;
        { // Filled portion(part that repesents "progress")
            _filledArea = dim;
            _filledArea.size.getDimension(_layoutDirection) = filledSize;
            Draw::AttributeGuard ag{ this, _colors.progressNormal };
            Draw::filledBlock( this, _filledArea );
        }

        { // Filled portion(part that repesents "remaining")
            _unfilledArea = dim;
            _unfilledArea.size.getDimension(_layoutDirection) = unfilledSize;
            _unfilledArea.topLeft.getPosition(_layoutDirection) += filledSize;
            Draw::AttributeGuard ag{ this, _colors.progressNormal };
            Draw::filledBlock( this, _unfilledArea );
        }

    }
}
}
