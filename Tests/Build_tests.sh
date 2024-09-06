#! /bin/bash



g++                                         \
    ./src/Preprocessor/ElmCoords.cpp     \
    ./src/Preprocessor/LCTsPhase/LCTsPhase.cpp     \
    ./src/Preprocessor/CleanupPhase/cleanupPhase.cpp     \
    ./src/Preprocessor/IncludePhase/includePhase.cpp     \
    ./src/Preprocessor/preprocessor.cpp     \
    ./Tests/tests.cpp                       \
    ./Tests/UnitTest.cpp                    \
    ./src/Utils/utils.cpp                   \
    ./src/ALC.cpp                           \
-std=c++20                                  \
-O0 -g                                                         \
-I                                          \
    ./src                                   \
-o                                          \
    ./Tests/tests.out