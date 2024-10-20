#pragma once
#include "ALC.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"








namespace cmp {
    // Base patterns. Doesn't include the root class.
    #define LIST_PATTERN_BASES_TYPES_NAMES                  \
        X(__base_Pattern_Token,        Token)               \
        X(__base_Pattern_Composite,    Composite)           \
        X(__base_Pattern_Operator,     Operator)

    // Pattern operators. These patterns cannot be cached and they can't generate a tree. They are only used to specify the syntax.
    #define LIST_PATTERN_OPERATOR_TYPES_NAMES               \
        X(__Pattern_Operator_Loop,     Loop)                \
        X(__Pattern_Operator_OneOf,    OneOf)               \
        X(__Pattern_Operator_Optional, Optional)


    // First-level patterns. These only include single-token patterns.
    #define LIST_PATTERN_TOKENS_TYPES_NAMES                 \
        X(Pattern_Keyword,             Keyword)             \
        X(Pattern_Identifier,          Identifier)          \
        X(Pattern_Literal,             Literal)


    // Element patterns. These identify actual semantic elements in the code and can generate trees.
    #define LIST_PATTERN_ELM_TYPES_NAMES                    \
        X(Pattern_Elm_Path,            Path)                \
        X(Pattern_Elm_Module,          Module)              \
        X(Pattern_Elm_Alias,           Alias)               \
        X(Pattern_Elm_Namespace,       Namespace)           \
        \
        X(Pattern_Elm_Type,            Type)                \
        X(Pattern_Elm_Type_Basic,      BasicType)           \
        X(Pattern_Elm_Type_Function,   FunctionType)        \
        \
        X(Pattern_Elm_Enum,            Enum)                \
        X(Pattern_Elm_EnumElm,         EnumElm)             \
        \
        X(Pattern_Elm_Struct,          Struct)





    // Pattern struct signatures
    #define X(type, name) \
        struct type;
    LIST_PATTERN_OPERATOR_TYPES_NAMES
    LIST_PATTERN_BASES_TYPES_NAMES
    LIST_PATTERN_TOKENS_TYPES_NAMES
    LIST_PATTERN_ELM_TYPES_NAMES
    #undef X



//FIXME make namespaces only allow what their parent allows
//FIXME make namespaces only allow what their parent allows
//FIXME make namespaces only allow what their parent allows
//FIXME make namespaces only allow what their parent allows




    // Base Type
    struct __base_Pattern {
        virtual ~__base_Pattern() = default;

        // Define isType and asType functions
        #define X(type, name) \
            const type *as##name() const; \
            /**/  type *as##name()      ; \
            /**/  bool  is##name() const; \
            /**/  bool  is##name()      ;
        LIST_PATTERN_OPERATOR_TYPES_NAMES
        LIST_PATTERN_BASES_TYPES_NAMES
        LIST_PATTERN_TOKENS_TYPES_NAMES
        #undef X


        virtual std::string genDecoratedValue(bool article) const = 0;
        virtual ulong getCertaintyThreshold() const = 0;
    };

}


//FIXME add "null" literal for pointers

//TODO ?        t (bool c) ? (t? value) { if(c) return $value; else return null; }
//TODO :        t (t? $a) : (t@ b) { if(a) return @a; else return b; }

//TODO [ takes a container and an index. it returns a pointer to the element that is at index <index>
//TODO ] takes a pointer and returns a reference to its value