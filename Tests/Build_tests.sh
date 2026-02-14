#! /bin/bash



g++                                         \
    ./src/Preprocessor/ElmCoords.cpp        \
    ./src/Utils/ansi.cpp                    \
    ./src/Utils/utils.cpp                   \
    ./src/Utils/format.cpp                  \
    ./src/Utils/console.cpp                 \
    ./src/Main/errors.cpp                   \
    ./src/Main/ALC.cpp                      \
    \
    ./Tests/tests.cpp                       \
    ./Tests/UnitTest.cpp                    \
-std=c++20                                  \
-O0 -g                                      \
-I                                          \
    ./src                                   \
-o                                          \
    ./Tests/tests.out