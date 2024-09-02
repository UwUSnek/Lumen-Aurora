#! /bin/bash



mkdir -p ./Build


g++                                                            \
    ./src/Preprocessor/CleanupPhase/UTF8_decoder.c             \
    ./src/Utils/utils.cpp                                      \
    ./src/Utils/DynamicProgressBar.cpp                         \
    ./src/main.cpp                                             \
    \
    ./src/Command/command.cpp                                  \
    \
    ./src/Preprocessor/preprocessor.cpp                        \
    ./src/Preprocessor/IntermediateCodeFormat.cpp              \
    ./src/Preprocessor/CleanupPhase/SegmentedCleanSource.cpp   \
    ./src/Preprocessor/ElmCoords.cpp                           \
    \
    ./src/Preprocessor/LCTsPhase/LCTsPhase.cpp                 \
    ./src/Preprocessor/CleanupPhase/cleanupPhase.cpp           \
    ./src/Preprocessor/IncludePhase/includePhase.cpp           \
    \
-std=c++20                                                     \
-O0 -g                                                         \
-I                                                             \
    ./src/                                                     \
-o                                                             \
    ./Build/out


#TODO add a separate build for the optimized version, maybe wait til rewrite