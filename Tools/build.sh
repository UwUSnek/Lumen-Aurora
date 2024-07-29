#! /bin/bash



mkdir -p ./Build
g++                                             \
-std=c++20                                      \
    ./src/utils.cpp                             \
    ./src/main.cpp                              \
    ./src/Preprocessor/preprocessor.cpp         \
-I                                              \
    ./src/                                      \
-o                                              \
    ./Build/out