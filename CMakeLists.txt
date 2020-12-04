cmake_minimum_required( VERSION 3.10 )
project( TextAnalysis VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

include_directories( ./include ./src )

set (core-files main.cpp analyzer.h analyzer.cpp )

add_executable( textanalysis ./src/main.cpp )

set ( THREADS_PREFER_PTHREAD_FLAG ON)
find_package( Threads REQUIRED )
target_link_libraries( textanalysis Threads::Threads )

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options( -Wall -Wextra -Wunreachable-code -Wpedantic )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options( /W4 )
endif()