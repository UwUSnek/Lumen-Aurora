#! /bin/bash



mkdir -p ./Build
g++                                             \
-std=c++20                                      \
    ./src/Utils/utils.cpp                       \
    ./src/main.cpp                              \
    ./src/Preprocessor/preprocessor.cpp         \
    ./src/Preprocessor/include.cpp              \
-I                                              \
    ./src/                                      \
-o                                              \
    ./Build/out