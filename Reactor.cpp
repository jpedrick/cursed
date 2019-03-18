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
#include "Reactor.hpp"
#include "IFileEventHandler.hpp"
#include <sys/select.h>
#include <array>
extern "C"{
#include <strings.h>
}

namespace cursed{

Reactor::Reactor(){
}

void Reactor::addFileEventHandler( IFileEventHandler* eventHandler ){
    _eventHandlers.push_back(eventHandler);
}

void Reactor::poll(){
    fd_set fdset;
    FD_ZERO(&fdset);
    int maxFd = -1;
    for( size_t i = 0; i < _eventHandlers.size(); ++i ){
        IFileEventHandler* eh = _eventHandlers[i];
        FD_SET(eh->fileDescriptor(), &fdset );
        maxFd = std::max( maxFd, eh->fileDescriptor() );
    }

    int n = 0;
    struct timeval timeout;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    n = ::select( maxFd + 1, &fdset, NULL, NULL, &timeout);
    if( n > 0 ){
        for( size_t i = 0; i < _eventHandlers.size(); ++i ){
            IFileEventHandler* eh = _eventHandlers[i];
            if( FD_ISSET( eh->fileDescriptor(), &fdset ) ){
                eh->onEvent( 0 );
            }
        }
    }
}

}
