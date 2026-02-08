
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

    #../src/Preprocessor
        ../src/Preprocessor/preprocessor.cpp
        ../src/Preprocessor/SegmentedCleanSource.cpp
        ../src/Preprocessor/ElmCoords.cpp
            #../src/Preprocessor/Phases
                #../src/Preprocessor/Phases/LCTs
                    ../src/Preprocessor/Phases/LCTs/LCTsPhase.cpp
                #../src/Preprocessor/Phases/Cleanup
                    ../src/Preprocessor/Phases/Cleanup/cleanupPhase.cpp
                #../src/Preprocessor/Phases/Include
                    ../src/Preprocessor/Phases/Include/includePhase.cpp
                    ../src/Preprocessor/Phases/Include/pathSolver.cpp
                #../src/Preprocessor/Phases/Macros
                    ../src/Preprocessor/Phases/Macros/macroPhase.cpp


    #../src/Compiler
        ../src/Compiler/compiler.cpp
            #../src/Compiler/Phases
                #../src/Compiler/Phases/Tokenization
                    ../src/Compiler/Phases/Tokenization/tokenizationPhase.cpp
                    ../src/Compiler/Phases/Tokenization/TokenizedSource.cpp
                    #../src/Compiler/Phases/Tokenization/Parsers
                        ../src/Compiler/Phases/Tokenization/Parsers/keywordsIdentifiers.cpp
                        ../src/Compiler/Phases/Tokenization/Parsers/textLiterals.cpp
                        ../src/Compiler/Phases/Tokenization/Parsers/numericalLiterals.cpp
../src/Compiler/Phases/Tokenization/whitespaceCounter.cpp
                #../src/Compiler/Phases/Tree
                    ../src/Compiler/Phases/Tree/treePhase.cpp
                    ../src/Compiler/Phases/Tree/SourceTree.cpp
                #../src/Compiler/Phases/Tree/Patterns
                    ../src/Compiler/Phases/Tree/Patterns/__base_Pattern.cpp
                    #../src/Compiler/Phases/Tree/Patterns/Composite
                        ../src/Compiler/Phases/Tree/Patterns/Composite/__base_Pattern_Composite.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Composite/Module.cpp
                        #../src/Compiler/Phases/Tree/Patterns/Composite/Elements
                            ../src/Compiler/Phases/Tree/Patterns/Composite/Elements/Namespace.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/Elements/Enum.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/Elements/Struct.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/Elements/Alias.cpp
                        #../src/Compiler/Phases/Tree/Patterns/Composite/Directives
                            ../src/Compiler/Phases/Tree/Patterns/Composite/Directives/Import.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/Directives/Export.cpp
                        #../src/Compiler/Phases/Tree/Patterns/Composite/SubElements
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/Path.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/EnumElm.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/StructElm.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/StructElmCluster.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/Type.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/Type_Basic.cpp
                            ../src/Compiler/Phases/Tree/Patterns/Composite/SubElements/Type_Function.cpp
                    #../src/Compiler/Phases/Tree/Patterns/Operators
                        ../src/Compiler/Phases/Tree/Patterns/Operators/__base_Pattern_Operator.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Operators/OperatorLoop.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Operators/OperatorOneOf.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Operators/OperatorOptional.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Operators/OperatorSequence.cpp
                    #../src/Compiler/Phases/Tree/Patterns/Tokens
                        ../src/Compiler/Phases/Tree/Patterns/Tokens/Identifier.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Tokens/Keyword.cpp
                        ../src/Compiler/Phases/Tree/Patterns/Tokens/Literal.cpp
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