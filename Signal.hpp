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
#include <atomic>
#include <optional>
#include <type_traits>

namespace cursed{

#define signal_field( XXX )

    struct IConnectionToken;

    template<typename PARENT_CLASS>
    struct ScopedBlockGuard{
        using ParentClass = PARENT_CLASS;

        ScopedBlockGuard( ParentClass* signal ) : _parent( signal->atomicBlock() ? signal : nullptr ) { }
        ScopedBlockGuard( ScopedBlockGuard&& moving ) : _parent( moving._parent ){
            moving._parent = nullptr;
        }

        ~ScopedBlockGuard(){
            if( _parent ){
                _parent->unblock();
            }
        }

        operator bool(){
            return _parent;
        }

    private:
        ParentClass* _parent;
    };

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

        virtual bool atomicBlock() = 0;
        virtual void unblock() = 0;
        virtual void clearSignal( ) = 0;

    protected:
        const uint64_t _key;
    };

    template< typename... ARGS >
    using OptionalFilterTuple = typename std::tuple< typename std::optional< typename std::decay<ARGS>::type>...>;


    template< typename... ARGS, size_t... Index >
    bool filter_matches( const OptionalFilterTuple<ARGS...>& filter, const std::tuple<ARGS...>& value, std::index_sequence<Index...> const& ){
        bool matches = ( ( std::get<Index>(filter) ? *std::get<Index>(filter) == std::get<Index>(value) : true ) && ... );

        return matches;
    }

    template< typename... ARGS >
    bool filter_matches( const OptionalFilterTuple<ARGS...>& filter, const std::tuple<ARGS...>& value ){
        return filter_matches( filter, value, std::make_index_sequence<sizeof...(ARGS)>() );
    }

    template< typename... ARGS >
    struct Signal {
        using filter_tuple = std::tuple< typename std::decay<ARGS>::type...>; 
        using optional_filter_tuple = std::tuple< typename std::optional< typename std::decay<ARGS>::type>...>; 
        using slot_function = std::function<void( ARGS... ) >;

        struct ConnectionToken : public IConnectionToken {
            friend struct Signal;
            using concrete_pointer = std::shared_ptr<ConnectionToken>;

            ConnectionToken( slot_function f, Signal* signal, uint64_t key ) : IConnectionToken(key), _function(f), _signal(signal) { }

            ConnectionToken() = delete;
            ConnectionToken( ConnectionToken&& token ) = delete;
            ConnectionToken( const ConnectionToken& ) = delete;

            ~ConnectionToken(){
                disconnect();
            }

            void disconnect() override { 
                if( _signal ){ // destructing(): avoid double free
                    _signal->disconnect( *this ); 
                }
            }

            bool isConnected() const override{ 
                if( _signal ){ 
                    return _signal->isConnected( *this );
                }

                return false;
            }
            IConnectionToken::pointer reconnect() override{ return _signal->reconnect( *this ); }

            void clearSignal( ) override{
                _signal = nullptr;
            }

            template< typename... CALLARGS >
            inline void call( CALLARGS&&... args ){
                _function( std::forward<CALLARGS>(args)... );
            }

            bool atomicBlock() { return !_blocked.exchange(true); }
            void unblock() { _blocked = false; }

            ScopedBlockGuard<ConnectionToken> block(){
                return ScopedBlockGuard<ConnectionToken>{this};
            }

        private:
            const slot_function _function;
            Signal* _signal;
            std::atomic<bool> _blocked = false;
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

            add(token);

            return token;
        }

        std::shared_ptr<IConnectionToken> connect( slot_function f, const optional_filter_tuple& filter ){ 
            auto filteredFunction = [f,filter]( ARGS&&... args ){
                if( filter_matches( filter, std::make_tuple( std::forward< typename std::remove_reference<ARGS>::type>(args)... ) ) ){
                    f( std::forward<ARGS>(args)... );
                }
            };

            return connect( filteredFunction );
        }

        ScopedBlockGuard<Signal> block(){
            return ScopedBlockGuard<Signal>{ this };
        }

        // returns true if aquired
        bool atomicBlock( ){ return !_blocked.exchange( true ); } 

        bool unblock( ){ return _blocked = false; }

        template< typename... CALLARGS >
        void emit( CALLARGS&&... args ){
            if( !_blocked ){
                for( auto& token : _tokens ) {
                    token.second->call( std::forward<CALLARGS>(args)... );
                }
            }
        }

        template< typename... CALLARGS >
        void emitAfter( const std::function<void()>& preEmit, CALLARGS&&... args ){
            // Delegate to internal function to ensure references are removed prior to preEmit's invocation
            // this allows you to avoid creating temporary variables for new/old value-changed type signals
            emitAfterInternal< typename std::remove_reference<CALLARGS>::type... >( preEmit, args... );
        }

        template< typename... CALLARGS >
        void operator()( CALLARGS&&... args ){
            emit( std::forward<CALLARGS>(args)... );
        }

    protected:

        void add( const std::shared_ptr<ConnectionToken>& token ) { 
            if( token ){
                _tokens.emplace( token->key(), token );
            }
        }

        IConnectionToken::pointer reconnect( const ConnectionToken& token ){ 
            if( !isConnected( token ) ){
                return connect( token._function );
            }

            return {};
        }

        bool isConnected( const ConnectionToken& token ) const {
            return _tokens.count( token.key() ) > 0;
        }

        void disconnect( const ConnectionToken& token ){ 
            if ( isConnected( token ) ){
                _tokens.erase( token.key() );
            }
        }

        bool destructing() const {
            return _destructing;
        }

    private:
        template< typename... CALLARGS >
        void emitAfterInternal( const std::function<void()>& preEmit, typename std::remove_reference<CALLARGS>::type... args ){
            preEmit();
            emit( std::forward<CALLARGS>(args)... );
        }

        std::atomic<bool> _blocked = false;
        bool _destructing = false;
        uint64_t _connectionCounter = 0;

        std::unordered_map<uint64_t, std::shared_ptr<ConnectionToken> > _tokens;
    };
}

