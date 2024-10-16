#pragma once
#include "ALC.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"








namespace cmp {
    // Base patterns. Doesn't include the root class.
    #define LIST_PATTERN_BASES_TYPES_NAMES         \
        X(__base_Pattern_Token,        Token)      \
        X(__base_Pattern_Composite,    Composite)  \
        X(__base_Pattern_Operator,     Operator)

    // Pattern operators. These patterns cannot be cached and they can't generate a tree. They are only used to specify the syntax.
    #define LIST_PATTERN_OPERATOR_TYPES_NAMES      \
        X(__Pattern_Operator_Loop,     Loop)       \
        X(__Pattern_Operator_OneOf,    OneOf)      \
        X(__Pattern_Operator_Optional, Optional)


    // First-level patterns. These only include single-token patterns.
    #define LIST_PATTERN_TOKENS_TYPES_NAMES        \
        X(Pattern_Keyword,             Keyword)    \
        X(Pattern_Identifier,          Identifier) \
        X(Pattern_Literal,             Literal)


    // Element patterns. These identify actual semantic elements in the code and can generate trees.
    #define LIST_PATTERN_ELM_TYPES_NAMES           \
        X(Pattern_Elm_Path,            Path)       \
        X(Pattern_Elm_Module,          Module)     \
        X(Pattern_Elm_Alias,           Alias)      \
        X(Pattern_Elm_Namespace,       Namespace)  \
        X(Pattern_Elm_Enum,            Enum)       \
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


        virtual std::string genDecoratedValue() const = 0;
        virtual ulong getCertaintyThreshold() const = 0;
    };

















    struct __base_Pattern_Token : public virtual __base_Pattern {
    };




    struct __base_Pattern_Operator : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;

        template<class ...t> void __internal_init(t... _v) {
            v = std::vector<__base_Pattern*>{ dynamic_cast<__base_Pattern*>(_v)... };
        }
    };




    debug(
        static std::string __internal_repeat(std::string s, ulong n) {
            std::stringstream r;
            for(ulong i = 0; i < n; ++i) r << s;
            return r.str();
        }
    )
    debug(void printPatternElmInfo(__base_Pattern* p, int indent);)
    struct __base_Pattern_Composite : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;
        template<class ...t> void __internal_init(t... _v) {
            v = std::vector<__base_Pattern*>{ dynamic_cast<__base_Pattern*>(_v)... };
            debug(
                cout++;
                cout << "\n" << this << " Composite pattern initialized with:\n";
                for(ulong i = 0; i < v.size(); ++i) {
                    printPatternElmInfo(v[i], 1);
                }
                cout << "\n";
                cout--;
            );
        }

        // virtual bool isChildAllowed(__base_ST* const child) const = 0;
        virtual __base_ST* generateData(std::vector<__base_ST*> const &parsedElements) const = 0;
    };








    struct __Pattern_Operator_Loop : public virtual __base_Pattern_Operator {
        template<class ...t> void init(t... _v) {
            __base_Pattern_Operator::__internal_init(_v...);
        }

        std::string genDecoratedValue() const override { return "" debug("Debug:OperatorLoop"); }
        ulong getCertaintyThreshold() const override { return 0; } //TODO check if this is ok
    };




    struct __Pattern_Operator_OneOf : public virtual __base_Pattern_Operator {
        template<class ...t> void init(t... _v) {
            __base_Pattern_Operator::__internal_init(_v...);
            debug(for(auto p : v) if(p->isOptional()) (cout++ << "Optional operator used as direct child of OneOf. This is not allowed.")--;)
        }

        std::string genDecoratedValue() const override { return "" debug("Debug:OperatorOneOf"); }
        ulong getCertaintyThreshold() const override { return 0; } //TODO check if this is ok
    };




    struct __Pattern_Operator_Optional : public virtual __base_Pattern_Operator {
        template<class ...t> void init(t... _v) {
            __base_Pattern_Operator::__internal_init(_v...);
        }

        std::string genDecoratedValue() const override { return "" debug("Debug:OperatorOptional"); }
        ulong getCertaintyThreshold() const override { return 0; } //TODO check if this is ok
    };








    struct Pattern_Keyword : public virtual __base_Pattern_Token {
        ReservedTokenId id;
        void init(ReservedTokenId _id){
            id = _id;
        }

        std::string genDecoratedValue() const override {
            return "The keyword \"" + std::find_if(
                reservedTokensMap.begin(),
                reservedTokensMap.end(),
                [&](const auto& pair) { return pair.second == id; }
            )->first + "\"";
        }
        ulong getCertaintyThreshold() const override {
            return 1;
        }
    };


    struct Pattern_Identifier : public virtual __base_Pattern_Token {
        void init(){}

        std::string genDecoratedValue() const override {
            return "An identifier";
        }
        ulong getCertaintyThreshold() const override {
            return 1;
        }
    };


    struct Pattern_Literal : public virtual __base_Pattern_Token {
        void init(){}

        std::string genDecoratedValue() const override {
            return "A literal";
        }
        ulong getCertaintyThreshold() const override {
            return 1;
        }
    };












    //TODO rename to "root"
    struct Pattern_Elm_Module : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;

        std::string genDecoratedValue() const override { return "" debug("Debug:Module"); }
        ulong getCertaintyThreshold() const override;
        // virtual bool isChildAllowed(__base_ST* const c) const {
            // return !c->isStatement();
        // }
    };
}


//FIXME add "null" literal for pointers

//TODO ?        t (bool c) ? (t? value) { if(c) return $value; else return null; }
//TODO :        t (t? $a) : (t@ b) { if(a) return @a; else return b; }

//TODO [ takes a container and an index. it returns a pointer to the element that is at index <index>
//TODO ] takes a pointer and returns a reference to its value