
# C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)




# Source files
add_executable(alc


#../src

    #../src/Main
        ../src/Main/ALC.cpp
        ../src/Main/monitorThread.cpp
        ../src/Main/main.cpp
        ../src/Main/errors.cpp


    #../src/Generated
        ../src/Generated/buildNumber.cpp


    #../src/utils
        ../src/Utils/utils.cpp
        ../src/Utils/ansi.cpp
        ../src/Utils/DynamicProgressBar.cpp
        ../src/Utils/format.cpp
        ../src/Utils/console.cpp


    #../src/Command
        ../src/Command/command.cpp
        ../src/Command/info.cpp


    #../src/Misc
        ../src/Misc/CommentCounter.cpp
        ../src/Misc/LstCounter.cpp
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
                #../src/Preprocessor/Phases/1-LineSplicing
                    ../src/Preprocessor/Phases/1-LineSplicing/LineSplicingPhase.cpp
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
target_link_options(alc PRIVATE
    $<$<CONFIG:Debug>:
        -fsanitize=address      # Check Heap/stack overflows, use-after-free  # Incompatible with Valgrind
        -fsanitize=undefined    # Check UB: signed overflow, null deref, misaligned access...
        -fsanitize=leak         # Check Memory leaks (included in address on Linux, but explicit is fine)
        # -fsanitize=thread       # Check thread issues  # Incompatible with the other sanitizer options
    >
    $<$<CONFIG:Release>:
        -flto                   # Link-time optimizations - increases linking step times for runtime speed
    >
)
target_compile_options(alc PRIVATE
    $<$<CONFIG:Debug>:
        -O0                     # No optimizations
        -g3                     # Save all debug infos
        -fno-omit-frame-pointer # Reliable stack traces
        -fno-inline             # Don't inline functions

        -fsanitize=address      # Check Heap/stack overflows, use-after-free  # Incompatible with Valgrind
        -fsanitize=undefined    # Check UB: signed overflow, null deref, misaligned access...
        -fsanitize=leak         # Check Memory leaks (included in address on Linux, but explicit is fine)
        # -fsanitize=thread       # Check thread issues  # Incompatible with the other sanitizer options

        -Wall                   # Common warnings
        -Wextra                 # Extra warnings
        -Wpedantic              # Pedantic warnings
        -Wshadow                # Variable shadowing
        -Wconversion            # Implicit type conversions
        -Wnull-dereference      # Potential null dereference
        -Wformat=2              # Potential format string issues
    >
    $<$<CONFIG:Release>:
        -O3                     # Maximum optimizations
        -flto                   # Link-time optimizations - increases linking step times for runtime speed
        -DNDEBUG                # Disable asserts
    >
)




# Include paths
target_include_directories(alc PRIVATE ../src/)