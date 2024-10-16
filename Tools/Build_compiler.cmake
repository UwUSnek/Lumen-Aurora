
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)




# Source files
add_executable(alc
    ../src/Generated/buildNumber.cpp

    ../src/Compiler/UTF8_decoder.c
    ../src/Utils/utils.cpp
    ../src/Utils/ansi.cpp
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
            ../src/Compiler/TokenizationPhase/Parsers/keywordsIdentifiers.cpp
            ../src/Compiler/TokenizationPhase/Parsers/textLiterals.cpp
            ../src/Compiler/TokenizationPhase/Parsers/numericalLiterals.cpp
            ../src/Compiler/TokenizationPhase/whitespaceCounter.cpp
        ../src/Compiler/TreePhase/SourceTree.cpp
        ../src/Compiler/TreePhase/TreePatterns.cpp
        ../src/Compiler/TreePhase/treePhase.cpp
            ../src/Compiler/TreePhase/Parsers/Elements/namespace.cpp
            ../src/Compiler/TreePhase/Parsers/Elements/enum.cpp
            ../src/Compiler/TreePhase/Parsers/Elements/struct.cpp
            ../src/Compiler/TreePhase/Parsers/Elements/alias.cpp
            ../src/Compiler/TreePhase/Parsers/Generic/scope.cpp
            ../src/Compiler/TreePhase/Parsers/Directives/import.cpp
            ../src/Compiler/TreePhase/Parsers/Directives/export.cpp
            ../src/Compiler/TreePhase/Parsers/SubElements/path.cpp
)




# Set output directory for the executable
set_target_properties(alc PROPERTIES RUNTIME_OUTPUT_DIRECTORY ../Build)




# GCC compilaiton options
target_compile_options(alc PRIVATE
    -O0         # No optimizations
    -g          # Save debug infos
)




# Include paths
target_include_directories(alc PRIVATE ../src/)