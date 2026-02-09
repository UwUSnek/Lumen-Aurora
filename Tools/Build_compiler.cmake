
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)




# Source files
add_executable(alc


#../src
    ../src/ALC.cpp
    ../src/monitorThread.cpp
    ../src/main.cpp


    #../src/Generated
        ../src/Generated/buildNumber.cpp


    #../src/utils
        ../src/Utils/utils.cpp
        ../src/Utils/ansi.cpp
        ../src/Utils/errors.cpp
        ../src/Utils/DynamicProgressBar.cpp
        ../src/Utils/format.cpp


    #../src/Command
        ../src/Command/command.cpp
        ../src/Command/info.cpp


    #../src/Misc
        ../src/Misc/CommentCounter.cpp
        ../src/Misc/LctCounter.cpp
        ../src/Misc/TextLiteralCounter.cpp
        ../src/Misc/whitespaceCounter.cpp


    #../src/Preprocessor
        ../src/Preprocessor/preprocessor.cpp
        ../src/Preprocessor/SegmentedCleanSource.cpp
        ../src/Preprocessor/ElmCoords.cpp
            #../src/Preprocessor/Phases
                #../src/Preprocessor/Phases/0-Include
                    ../src/Preprocessor/Phases/0-Include/includePhase.cpp
                    ../src/Preprocessor/Phases/0-Include/pathSolver.cpp
                    ../src/Preprocessor/Phases/0-Include/metadataGenerator.cpp
                #../src/Preprocessor/Phases/1-LCTs
                    ../src/Preprocessor/Phases/1-LCTs/LCTsPhase.cpp
                #../src/Preprocessor/Phases/2-Cleanup
                    ../src/Preprocessor/Phases/2-Cleanup/cleanupPhase.cpp
                #../src/Preprocessor/Phases/3-Macros
                    ../src/Preprocessor/Phases/3-Macros/macroPhase.cpp


    #../src/Compiler
        ../src/Compiler/compiler.cpp
        ../src/Compiler/UTF8_decoder.c
            #../src/Compiler/Phases
                #../src/Compiler/Phases/0-Tokenization
                    ../src/Compiler/Phases/0-Tokenization/tokenizationPhase.cpp
                    ../src/Compiler/Phases/0-Tokenization/TokenizedSource.cpp
                    #../src/Compiler/Phases/0-Tokenization/Parsers
                        ../src/Compiler/Phases/0-Tokenization/Parsers/keywordsIdentifiers.cpp
                        ../src/Compiler/Phases/0-Tokenization/Parsers/textLiterals.cpp
                        ../src/Compiler/Phases/0-Tokenization/Parsers/numericalLiterals.cpp
                #../src/Compiler/Phases/1-Tree
                    ../src/Compiler/Phases/1-Tree/treePhase.cpp
                    ../src/Compiler/Phases/1-Tree/SourceTree.cpp
                #../src/Compiler/Phases/1-Tree/Patterns
                    ../src/Compiler/Phases/1-Tree/Patterns/__base_Pattern.cpp
                    #../src/Compiler/Phases/1-Tree/Patterns/Composite
                        ../src/Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Composite/Module.cpp
                        #../src/Compiler/Phases/1-Tree/Patterns/Composite/Elements
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/Elements/Namespace.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/Elements/Enum.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/Elements/Struct.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/Elements/Alias.cpp
                        #../src/Compiler/Phases/1-Tree/Patterns/Composite/Directives
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/Directives/Import.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/Directives/Export.cpp
                        #../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/Path.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/EnumElm.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/StructElm.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/StructElmCluster.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/Type.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/Type_Basic.cpp
                            ../src/Compiler/Phases/1-Tree/Patterns/Composite/SubElements/Type_Function.cpp
                    #../src/Compiler/Phases/1-Tree/Patterns/Operators
                        ../src/Compiler/Phases/1-Tree/Patterns/Operators/__base_Pattern_Operator.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Operators/OperatorLoop.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Operators/OperatorOneOf.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Operators/OperatorOptional.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Operators/OperatorSequence.cpp
                    #../src/Compiler/Phases/1-Tree/Patterns/Tokens
                        ../src/Compiler/Phases/1-Tree/Patterns/Tokens/Identifier.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Tokens/Keyword.cpp
                        ../src/Compiler/Phases/1-Tree/Patterns/Tokens/Literal.cpp
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