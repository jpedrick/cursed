cmake_minimum_required( VERSION 3.2 )

set(CMAKE_CXX_COMPILER g++)
set(CMAKE_C_COMPILER gcc)
set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

project( cursed CXX )
include( CTest )

#set(CURSES_NEED_NCURSES true)
#set(CURSES_NEED_WIDE    true)

#find_package(Curses REQUIRED)
list( APPEND DEPENDENCIES rt  dl anl pthread z ncursesw )

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11 -Wpedantic -Wuninitialized -Wmissing-field-initializers -Werror -ftrack-macro-expansion=0")
set(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -pthread -std=c++11")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -std=c++11")

set(TEMPLATE lib)
set(TARGET cursed)

exec_program( git ls-files *.cpp
             OUTPUT_VARIABLE GIT_CPPS)
message( git cpps: ${GIT_CPPS} )         

file( GLOB HEADERS *.hpp )
file( GLOB SOURCES *.cpp )

foreach( src ${SOURCES} )
    get_filename_component(base_src ${src} NAME )
    if( ${base_src} MATCHES "^test_.*\.cpp$" )
        list(APPEND TEST_SOURCES ${src} )
        list(REMOVE_ITEM SOURCES ${src} )
    endif()
endforeach(src)

foreach( src ${SOURCES} )
    get_filename_component(base_src ${src} NAME )
    if( ${base_src} MATCHES "^example_.*\.cpp$" )
        list(APPEND EXAMPLE_SOURCES ${src} )
        list(REMOVE_ITEM SOURCES ${src} )
    endif()
endforeach(src)

add_library( ${TARGET} ${SOURCES} ${HEADERS} )
target_link_libraries( ${TARGET} ${DEPENDENCIES} )

if( TEST_SOURCES )
    if ( NOT TEST_DEPENDENCIES )
        set( TEST_DEPENDENCIES ${DEPENDENCIES} )
    endif()
    foreach( test_source ${TEST_SOURCES} )
        get_filename_component( test_name ${test_source} NAME_WE )

        set(TEST_EXECUTABLE ${TARGET}_${test_name} )

        add_executable( ${TEST_EXECUTABLE} ${test_source} )
        target_link_libraries( ${TEST_EXECUTABLE} ${TARGET} ${TEST_DEPENDENCIES} ${DEPENDENCIES} )
        add_test( ${TEST_EXECUTABLE} ${TEST_EXECUTABLE} )
    endforeach(test_source)
endif()

if( EXAMPLE_SOURCES )
    foreach( example_source ${EXAMPLE_SOURCES} )
        get_filename_component( example_name ${example_source} NAME_WE )

        set(EXAMPLE_EXECUTABLE ${example_name} )

        add_executable( ${EXAMPLE_EXECUTABLE} ${example_source} )
        target_link_libraries( ${EXAMPLE_EXECUTABLE} ${TARGET} ${EXAMPLE_DEPENDENCIES} ${DEPENDENCIES} )
    endforeach(example_source)
endif()
