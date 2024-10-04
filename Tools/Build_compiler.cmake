
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)




# Source files
add_executable(alc
    ../src/Compiler/UTF8_decoder.c
    ../src/Utils/utils.cpp
    ../src/Utils/errors.cpp
    ../src/Utils/DynamicProgressBar.cpp
    ../src/ALC.cpp
    ../src/monitorThread.cpp
    ../src/main.cpp

    ../src/Command/command.cpp
    ../src/Command/info.cpp


    ../src/Preprocessor/preprocessor.cpp
    ../src/Preprocessor/SegmentedCleanSource.cpp
        ../src/Preprocessor/ElmCoords.cpp
        ../src/Preprocessor/LCTsPhase/LCTsPhase.cpp
        ../src/Preprocessor/CleanupPhase/cleanupPhase.cpp
        ../src/Preprocessor/IncludePhase/includePhase.cpp
            ../src/Preprocessor/IncludePhase/pathSolver.cpp
        ../src/Preprocessor/MacroPhase/macroPhase.cpp

    ../src/Compiler/compiler.cpp
        ../src/Compiler/TokenizationPhase/TokenizedSource.cpp
        ../src/Compiler/TokenizationPhase/tokenizationPhase.cpp
            ../src/Compiler/TokenizationPhase/parseKeywordsIdentifiers.cpp
            ../src/Compiler/TokenizationPhase/parseTextLiterals.cpp
            ../src/Compiler/TokenizationPhase/parseNumericalLiterals.cpp
            ../src/Compiler/TokenizationPhase/whitespaceCounter.cpp
        ../src/Compiler/TreePhase/SourceTree.cpp
        ../src/Compiler/TreePhase/treePhase.cpp
)




# Set output directory for the executable
set_target_properties(alc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../Build)




# GCC compilaiton options
target_compile_options(alc PRIVATE -O0 -g)




# Include paths
target_include_directories(alc PRIVATE ../src/)