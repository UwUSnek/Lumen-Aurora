#! /bin/bash



mkdir -p ./Build
g++                                             \
    ./src/main.cpp                              \
    ./src/Preprocessor/Preprocessor.cpp         \
-I                                              \
    ./src/                                      \
-o                                              \
    ./Build/out