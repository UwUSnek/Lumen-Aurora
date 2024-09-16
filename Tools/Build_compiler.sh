#! /bin/bash



mkdir -p ./Build


g++                                                                                    \
    ./src/Compiler/UTF8_decoder.c                                                      \
    ./src/Utils/utils.cpp                                                              \
    ./src/Utils/errors.cpp                                                             \
    ./src/Utils/DynamicProgressBar.cpp                                                 \
    ./src/ALC.cpp                                                                      \
    ./src/main.cpp                                                                     \
    \
    ./src/Command/command.cpp                                                          \
    ./src/Command/info.cpp                                                             \
    \
    \
    ./src/Preprocessor/preprocessor.cpp                                                \
    ./src/Preprocessor/SegmentedCleanSource.cpp                                        \
        ./src/Preprocessor/ElmCoords.cpp                                               \
        ./src/Preprocessor/LCTsPhase/LCTsPhase.cpp                                     \
        ./src/Preprocessor/CleanupPhase/cleanupPhase.cpp                               \
        ./src/Preprocessor/IncludePhase/includePhase.cpp                               \
            ./src/Preprocessor/IncludePhase/pathSolver.cpp                             \
        ./src/Preprocessor/MacroPhase/macroPhase.cpp                                   \
    \
    ./src/Compiler/compiler.cpp                                                        \
    ./src/Compiler/TokenizedSource.cpp                                                 \
        ./src/Compiler/TokenizationPhase/tokenizationPhase.cpp                         \
            ./src/Compiler/TokenizationPhase/keywordIdentifierParser.cpp               \
            ./src/Compiler/TokenizationPhase/literalParser.cpp                         \
            ./src/Compiler/TokenizationPhase/whitespaceCounter.cpp                     \
    \
    \
-std=c++20                                                     \
-O0 -g                                                         \
-I                                                             \
    ./src/                                                     \
-o                                                             \
    ./Build/out


#TODO add a separate build for the optimized version, maybe wait til rewrite