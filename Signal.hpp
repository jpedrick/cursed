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
#include <memory>
#include <optional>

namespace cursed{

#define signal_field( XXX )

    struct IConnectionToken;

    struct IConnectionToken{
        using pointer = std::shared_ptr<IConnectionToken>;

        IConnectionToken( uint64_t key ) : _key(key){}

        IConnectionToken( const IConnectionToken& ) = delete;
        IConnectionToken( IConnectionToken&& token ) : _key(token._key) {
        }

        virtual ~IConnectionToken(){ 
        }

        uint64_t key() const { return _key; }

        virtual void disconnect() = 0;
        virtual IConnectionToken::pointer reconnect() = 0;
        virtual bool isConnected() const = 0;

        virtual void clearSignal( ) = 0;

    protected:
        const uint64_t _key;
    };

    template< typename... ARGS >
    struct Signal {
        using args_tuple = std::tuple< typename std::decay<ARGS>::type...>; 
        using optional_args_tuple = std::tuple< typename std::optional< typename std::decay<ARGS>::type>...>; 
        using slot_function = std::function<void( ARGS... ) >;

        struct ConnectionToken : public IConnectionToken {
            friend struct Signal;

            ConnectionToken( slot_function f, Signal* signal, uint64_t key ) : IConnectionToken(key), function(f), signal(signal) { }

            ConnectionToken() = delete;
            ConnectionToken( ConnectionToken&& token ) = delete;
            ConnectionToken( const ConnectionToken& ) = delete;

            ~ConnectionToken(){
                disconnect();
            }

            void disconnect() override { 
                if( signal ){ // destructing(): avoid double free
                    signal->disconnect( *this ); 
                }
            }

            bool isConnected() const override{ 
                if( signal ){ 
                    return signal->isConnected( *this );
                }

                return false;
            }
            IConnectionToken::pointer reconnect() override{ return signal->reconnect( *this ); }

            void clearSignal( ) override{
                signal = nullptr;
            }

        private:
            const slot_function function;
            Signal* signal;
        };

        friend struct ConnectionToken;

        Signal( ) { }
        ~Signal( ) { 
            _destructing = true;
            for( auto& tok : _tokens ){
                tok.second->clearSignal(); // Make sure connection tokens don't call any operations on signal after signal is deleted
            }
        }

        std::shared_ptr<IConnectionToken> connect( const slot_function& f ){ 
            auto token = std::make_shared<ConnectionToken>( f, this, ++_connectionCounter );
            _slots.emplace( token->key(), f );

            add(token);

            return token;
        }

        std::shared_ptr<IConnectionToken> connect( slot_function f, const args_tuple& filter ){ 
            auto filteredFunction = [f,filter]( ARGS&&... args ){
                if( args_tuple( std::forward<ARGS>(args)... ) == filter ){
                    f( std::forward<ARGS>(args)... );
                }
            };

            return connect( filteredFunction );
        }

        // TODO: optional args comparison, if optional not set, skip from evaluation.
        std::shared_ptr<IConnectionToken> partial_connect( slot_function f, const optional_args_tuple& filter ){ 
            return {};
        }


        void block( bool v ){ _blocked = v; }

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


    protected:
        void add( const IConnectionToken::pointer& token ) { 
            if( token ){
                _tokens.emplace( token->key(), token );
            }
        }

        IConnectionToken::pointer reconnect( const ConnectionToken& token ){ 
            if( !isConnected( token ) ){
                return connect( token.function );
            }

            return {};
        }

        bool isConnected( const ConnectionToken& token ) const {
            return _slots.count( token.key() ) > 0;
        }

        void disconnect( const ConnectionToken& token ){ 
            if ( isConnected( token ) ){
                _slots.erase( token.key() ); 
                _tokens.erase( token.key() );
            }
        }

        bool destructing() const {
            return _destructing;
        }

    private:
        std::unordered_map<uint64_t, slot_function> _slots;
        bool _blocked = false;
        bool _destructing = false;
        uint64_t _connectionCounter = 0;

        std::unordered_map<uint64_t, IConnectionToken::pointer> _tokens;
    };
}

