cmake_minimum_required( VERSION 3.10 )
project( TextAnalysis VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 14 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

include_directories( ./include ./src )

add_executable( textanalysis ./src/main.cpp )

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options( -Wall -Wextra -Wunreachable-code -Wpedantic )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options( /W4 )
endif()