#! /bin/bash



mkdir -p ./Build


g++                                             \
    ./src/Utils/utils.cpp                       \
    ./src/main.cpp                              \
    ./src/Preprocessor/preprocessor.cpp         \
    ./src/Preprocessor/Cleanup/cleanup.cpp      \
    ./src/Preprocessor/Cleanup/ICF_Clean.cpp    \
    ./src/Preprocessor/ElmCoords.cpp            \
-std=c++20                                      \
-O0 -g                                          \
-I                                              \
    ./src/                                      \
-o                                              \
    ./Build/out


#TODO add a separate build for the optimized version, maybe wait til rewrite