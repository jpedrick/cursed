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
#include <stdexcept>
#include <sstream>
extern "C"{
#include <string.h>
struct epoll_event;
}

namespace cursed{

class Log;
class IWindow;
class IFileEventHandler;

}

#define cprint( __expression__ ) " " << #__expression__ << ":" << __expression__
#define cursed_out( __stream_expression__ ) ( ::cursed::Log::instance() << __stream_expression__ << "\t[" << __FILE__ << ":" << __LINE__ << "][" << __func__ << "]"  ).endl();
#define cursed_echo( COMMAND, OUTPUT ) { ( ::cursed::Log::instance() << #COMMAND << "[" << OUTPUT << "]" << "[" << __FILE__ << ":" << __LINE__ << "][" << __func__ << "]"  ).endl();} COMMAND
#define cursed_throw( DESCRIPTION ) { std::stringstream ss; ss << DESCRIPTION << "[" << __FILE__ << ":" << __LINE__ << "][" << __func__ << "]"; throw std::runtime_error( ss.str() ); }
#define cursed_assert( TRUE_STATEMENT, DESCRIPTION ) if( !(TRUE_STATEMENT) ){ cursed_out(DESCRIPTION); cursed_throw(DESCRIPTION); }
#define cursed_system_expect( __expression__ ){ \
    if( __expression__ ){ char errbuf[256]; \
        cursed_throw( cprint(#__expression__) << cprint(strerror_r(errno, errbuf, sizeof(errbuf))) ); \
    }\
}

#define unused(SYM) SYM __attribute__((unused))
