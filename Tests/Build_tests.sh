#! /bin/bash



g++                                         \
    ./src/Preprocessor/ElmCoords.cpp     \
    ./src/Utils/utils.cpp                   \
    ./src/Utils/errors.cpp                   \
    ./src/ALC.cpp                           \
    \
    ./Tests/tests.cpp                       \
    ./Tests/UnitTest.cpp                    \
-std=c++20                                  \
-O0 -g                                                         \
-I                                          \
    ./src                                   \
-o                                          \
    ./Tests/tests.out