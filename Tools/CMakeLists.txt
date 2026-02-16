cmake_minimum_required(VERSION 3.10)

# Project name and version
project(Aurora-Lumen-Compiler VERSION 1.0)


include(Build_compiler.cmake)


# Fix Clang not finding standard headers
if(CMAKE_EXPORT_COMPILE_COMMANDS)
    set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()