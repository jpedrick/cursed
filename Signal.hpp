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
#include <functional>
#include <unordered_map>

namespace cursed{

#define signal_field( XXX )

    template< typename... ARGS >
    struct Signal{
        typedef typename std::function<void( ARGS... ) > slot_function;

        // TODO: consider some kind of intrusive pointer/state so that if Signal is deleted connection tokens 
        // reflect that and do not segfault on disconnect call.
        struct ConnectionToken{
            ConnectionToken(){}
            ConnectionToken( slot_function f, Signal* signal, uint64_t key ) : 
                function(f), 
                signal(signal), 
                key(key)
            { }

            void disconnect(){
                if( signal ) signal->disconnect( key );
            }

            uint64_t key = 0;
        private:
            slot_function function = slot_function{};
            Signal* signal = nullptr;
        };

        friend struct ConnectionToken;

        Signal(){}
        ConnectionToken connect( slot_function f ){ 
            ConnectionToken token{ f, this, ++_connectionCounter };
            _slots.emplace( token.key, f );
            return token;
        }


        template< typename... CALLARGS >
        void emit( CALLARGS&&... args ){
            if( !_blocked ){
                for( auto& slot : _slots ) slot.second( std::forward<CALLARGS>(args)... );
            }
        }

        template< typename... CALLARGS >
        void operator()( CALLARGS&&... args ){
            emit( std::forward<CALLARGS>(args)... );
        }

        void block( bool v ){ _blocked = v; }

    protected:
        void disconnect( uint64_t key ){ _slots.erase(key); }
    private:
        std::unordered_map<uint64_t, slot_function> _slots;
        bool _blocked = false;
        uint64_t _connectionCounter = 0;
    };
}

