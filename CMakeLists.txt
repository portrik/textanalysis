cmake_minimum_required( VERSION 3.10 )
project( TextAnalysis VERSION 1.0.0 LANGUAGES CXX )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED ON )
set( CMAKE_CXX_EXTENSIONS OFF )

set(core-files
        ./src/cmdline.hpp
        ./src/cmdline.cpp
        ./src/analyzer.cpp
        ./src/analyzer.hpp
        ./src/statistics.cpp
        ./src/statistics.hpp
        ./src/word_cloud.hpp
        ./src/word_cloud.cpp)

if ( CMAKE_CXX_COMPILER_ID MATCHES "Clang|AppleClang|GNU")
    add_compile_options( -Wall -Wextra -Wunreachable-code -Wpedantic )
endif()
if ( CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    add_compile_options( /W4 )
endif()

add_executable( textanalysis ${core-files} ./src/main.cpp )