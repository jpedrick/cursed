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
#include "Timer.hpp"
#include "unistd.h"
#include "Forward.hpp"
#include "Log.hpp"

extern "C"{
#include <sys/timerfd.h>
}

namespace cursed{
    Timer::Timer( std::chrono::nanoseconds dur, bool oneShot ){
        _fd = timerfd_create( CLOCK_REALTIME, TFD_NONBLOCK | TFD_CLOEXEC );

        _timeout.it_interval.tv_nsec = (dur % std::chrono::seconds(1L)).count();
        _timeout.it_interval.tv_sec = dur.count() / ((std::chrono::nanoseconds)std::chrono::seconds{1L}).count();
        
        if( oneShot ){
            _timeout.it_value.tv_nsec = 0;
            _timeout.it_value.tv_sec = 0;
        } else{
            _timeout.it_value.tv_nsec = _timeout.it_interval.tv_nsec;
            _timeout.it_value.tv_sec = _timeout.it_interval.tv_sec;
        }

        ::timerfd_settime(_fd, 0, &_timeout, nullptr );
    }

    void Timer::onEvent( int ) {
        struct itimerspec t;
        ssize_t unused(bytesRead) = ::read( _fd, &t, sizeof(t) );

        signals.timeout.emit();
    }
}
