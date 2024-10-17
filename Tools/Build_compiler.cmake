
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
        ../src/Compiler/TreePhase/__base_Pattern.cpp
        ../src/Compiler/TreePhase/treePhase.cpp

            ../src/Compiler/TreePhase/Composite/Module.cpp
            ../src/Compiler/TreePhase/Composite/Elements/Namespace.cpp
            ../src/Compiler/TreePhase/Composite/Elements/Enum.cpp
            ../src/Compiler/TreePhase/Composite/Elements/Struct.cpp
            ../src/Compiler/TreePhase/Composite/Elements/Alias.cpp
            ../src/Compiler/TreePhase/Composite/Directives/Import.cpp
            ../src/Compiler/TreePhase/Composite/Directives/Export.cpp
            ../src/Compiler/TreePhase/Composite/SubElements/Path.cpp
            ../src/Compiler/TreePhase/Composite/SubElements/EnumElm.cpp
            ../src/Compiler/TreePhase/Composite/SubElements/Type.cpp
            ../src/Compiler/TreePhase/Composite/SubElements/Type_Basic.cpp
            ../src/Compiler/TreePhase/Composite/SubElements/Type_Function.cpp

            ../src/Compiler/TreePhase/Operators/OperatorLoop.cpp
            ../src/Compiler/TreePhase/Operators/OperatorOneOf.cpp
            ../src/Compiler/TreePhase/Operators/OperatorOptional.cpp

            ../src/Compiler/TreePhase/Tokens/Identifier.cpp
            ../src/Compiler/TreePhase/Tokens/Keyword.cpp
            ../src/Compiler/TreePhase/Tokens/Literal.cpp
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