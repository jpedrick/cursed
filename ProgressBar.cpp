
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
            progressBarSignals.clicked.emit( Area::Value, button );
        }
        else if( _unfilledArea.contains( relativePos ) ){
            progressBarSignals.clicked.emit( Area::Remaining, button );
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

int64_t ProgressBar::setValue( int64_t v ){
    const auto oldValue = value();
    _value = _valueRange.clip( v );
    draw( true );

    progressBarSignals.valueChanged( v, oldValue );

    return _value;
}

void ProgressBar::setValueRange( const Int64ValueRange& valueRange ){
    _valueRange = valueRange;

    setValue( value() ); //< in case value gets clipped, will also redraw
}

int64_t ProgressBar::adjustValue( int64_t change ){
    return setValue( change + value() );
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
            _filledArea.topLeft = Point{ 0, 0, dim.topLeft.z };
            //Draw::AttributeGuard ag{ this, ColorPair{ _colors.progressNormal, _colors.progressPressed } };
            //Draw::filledBlock( this, _filledArea );

            // clear button
            for( int i = 0; i < _filledArea.size.getDimension( _layoutDirection ); ++i ){
                Draw::line( this, other( _layoutDirection ), i, ' ');
            }
        }

        { // Filled portion(part that repesents "remaining")
            _unfilledArea = dim;
            _unfilledArea.size.getDimension(_layoutDirection) = unfilledSize;
            _unfilledArea.topLeft = _filledArea.topLeft;
            _unfilledArea.topLeft.getPosition(_layoutDirection) += filledSize;
            //Draw::AttributeGuard ag{ this, ColorPair{ _colors.remainingNormal, _colors.remainingPressed } };
            //Draw::filledBlock( this, _unfilledArea );

            // clear button
            for( int i = 0; i < _unfilledArea.size.getDimension( _layoutDirection ); ++i ){
                Draw::line( this, other( _layoutDirection ), i + filledSize, ' ');
            }
        }
        
        Draw::line( this, other(_layoutDirection ), filledSize, _layoutDirection == Direction::Vertical ? '-' : '|' );
        ::refresh();
    }
}
}
