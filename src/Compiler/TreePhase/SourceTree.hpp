#pragma once
#include <vector>
#include <string>

#include "ALC.hpp"
#include "Utils/Containers/GenericPipe.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"








namespace cmp {

    // struct Pattern_Identifier : public virtual __base_Pattern {
    //     bool parse() override {

    //     }
    // };

    // struct PatternOr : public virtual __base_PatternElm {
        // __base_PatternElm* a;
        // __base_PatternElm* b;
    // }





    #define LIST_SOURCE_TREE_TYPES_NAMES    \
        X(ST_Sub_Identifier, Identifier)    \
        X(ST_Sub_Type, Type)                \
        X(ST_Sub_Path, Path)                \
        \
        X(ST_Statement, Statement)          \
        \
        X(ST_Module, Module)                \
        X(ST_Namespace, Namespace)          \
        X(ST_Struct, Struct)                \
        X(ST_Enum, Enum)                    \
        X(ST_Alias, Alias)                  \
        \
        X(ST_Import, Import)                \
        X(ST_Export, Export)                \
        \
        X(ST_Variable, Variable)            \
        X(ST_Function, Function)            \
        X(ST_Operator, Operator)




    // Write struct signatures for the asType functions
    #define X(type, name) \
        struct type;
    LIST_SOURCE_TREE_TYPES_NAMES
    #undef X


    struct ST_Module;
    /**
     * @brief The base Source Tree structure
     */
    struct __base_ST {
        ulong tokenBgn;     // The index of the token in which this element begins
        ulong tokenEnd;     // The index of the token in which this element ends
        __base_ST* parent; //TODO set module's parent
        //TODO also set the root module's parent to NULL

        // std::vector<__base_PatternElm*> pattern;


        // virtual bool isChildAllowed(__base_ST* c)                const { return false; }
        virtual std::string getCategoryName(bool plural = false) const { return ""; }

        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~__base_ST() = default;




        #define X(type, name) \
            const type *as##name() const; \
            /**/  type *as##name()      ; \
            /**/  bool  is##name() const; \
            /**/  bool  is##name()      ;
        LIST_SOURCE_TREE_TYPES_NAMES
        #undef X
    };

    typedef GenericPipe<ST_Module*> SourceTree;








    // Wrapper for identifier tokens
    struct ST_Sub_Identifier : public virtual __base_ST {
        std::string s;
        std::string getCategoryName(bool plural = false) const override;

        ST_Sub_Identifier(std::string _s) :
            s(_s) {
        }
    };

    // Wrapper for keyword tokens
    struct ST_Sub_Keyword : public virtual __base_ST {
        ReservedTokenId id;
        std::string getCategoryName(bool plural = false) const override;

        ST_Sub_Keyword(ReservedTokenId _id) :
            id(_id) {
        }
    };

    // // Wrapper for literal tokens
    // struct ST_Sub_Identifier : public virtual __base_ST {
    //     std::string s;
    //     std::string getCategoryName(bool plural = false) const override;
    // };



    // Type sub-element
    struct ST_Sub_Type : public virtual __base_ST {
        bool isPointer;
        std::string getCategoryName(bool plural = false) const override;
    };

    // Basic types
    struct ST_Sub_Path;
    struct ST_Sub_Type_Basic : public virtual ST_Sub_Type {
        ST_Sub_Path *typeName;
    };

    // Function types
    struct ST_Sub_Type_Function : public virtual ST_Sub_Type {
        ST_Sub_Type *returnType;
        std::vector<ST_Sub_Type*> paramTypes;
    };




    // Complex symbol path sub-element (identifiers separated by a dot keyword)
    struct ST_Sub_Path : public virtual __base_ST {
        std::vector<std::string> idList;
        std::string getCategoryName(bool plural = false) const override;
    };











    // Base struct for every first-pass semantic element
    struct __base_ST_Container : public virtual __base_ST {
        std::vector<__base_ST*> children;

        void addChild(__base_ST* c) {
            children.push_back(c);
            c->parent = this;
        }
    };




    // Expressions
    //! The array of children contains the sub-expressions
    struct __base_ST_Container;
    struct ST_Expr : public virtual __base_ST_Container {
        std::string getCategoryName(bool plural = false) const override;
    };//FIXME parse this in the generic scope parser

    //FIXME basic expression types
        //FIXME path
            //TODO linkage phase:    operator call
            //TODO linkage phase:    function call
            //TODO linkage phase:    variable/parameter
            //TODO linkage phase:    enum value
        //FIXME reflection path
        //FIXME literal
        //FIXME (
        //FIXME )


    // Reflection path expression
    struct ST_Expr_Reflection : public virtual __base_ST {
        ST_Sub_Path* symbolPath;
        ST_Sub_Path* reflectionIdList;
    }; //FIXME parse this in the generic scope parser




    // Base struct for any semantic element that can be referenced by its name
    struct __base_ST_Referable : public virtual __base_ST {
        ST_Sub_Identifier *name;
    };


    struct ST_Sub_Path;
    // Base struct for any semantic element that has an explicit type
    struct __base_ST_Typed : public virtual __base_ST {
        ST_Sub_Type *type;
    };








    // Statements
    struct ST_Statement : public virtual __base_ST {
        // bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };//FIXME parse this in the generic scope parser

    //FIXME specialized statement types
        //FIXME expression statements
        //FIXME while
        //FIXME for
        //FIXME if
        //FIXME etc...








    struct ST_Module : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //TODO set the name in the treePhase parsing function
        //TODO take an alternative name from a parameter (for "import as"). in this case, the file's name can be invalid
        //TODO If not imported "as", the file name must be a valid identifier (after removing any extension)
        // bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };

    struct ST_Namespace : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        // bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };

    struct ST_Struct : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //FIXME struct members
        // bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };

    struct ST_Enum : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        ST_Sub_Path *baseType; //FIXME use type path
        //! Enum elements are saved in the children vector

        // bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };
    struct ST_Sub_EnumElement : public virtual __base_ST_Referable {
        //FIXME DEFAULT VALUE
    };

    struct ST_Alias : public virtual __base_ST_Referable {
        ST_Sub_Path *original = nullptr;
        std::string getCategoryName(bool plural = false) const override;
    };








    struct ST_Import : public virtual __base_ST {
        //FIXME whatever import needs
        std::string getCategoryName(bool plural = false) const override;
    };
    struct ST_Export : public virtual __base_ST {
        //FIXME whatever export needs
        std::string getCategoryName(bool plural = false) const override;
    };










    struct ST_Variable : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        //FIXME default value
        //FIXME const qualifier
        std::string getCategoryName(bool plural = false) const override;
    };


    //! Expressions are saved in the generic child array, together with the other elements
    struct __base_ST_Routine : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        //FIXME parameters
        //FIXME specialization constraint
    };
    struct ST_Function : public virtual __base_ST_Routine {
        std::string getCategoryName(bool plural = false) const override;
        // bool isChildAllowed(__base_ST* c) const override;
    };
    struct ST_Operator : public virtual __base_ST_Routine {
        //FIXME multiple names (or name position in case of split ternary)
        //FIXME priority
        std::string getCategoryName(bool plural = false) const override;
        // bool isChildAllowed(__base_ST* c) const override;

        //FIXME make [] use the normal syntax
    };















    // Pattern operators. These patterns cannot be cached and they can't generate a tree. They are only used to specify the syntax.
    #define LIST_PATTERN_OPERATOR_TYPES_NAMES      \
        X(__Pattern_Operator_Loop,  Loop)          \
        X(__Pattern_Operator_OneOf, OneOf)


    // Base patterns. Doesn't include the root class.
    #define LIST_PATTERN_BASES_TYPES_NAMES         \
        X(__base_Pattern_Token,     Token)         \
        X(__base_Pattern_Composite, Composite)     \


    // First-level patterns. These only include single-token patterns.
    #define LIST_PATTERN_TOKENS_TYPES_NAMES        \
        X(Pattern_Keyword,          Keyword)       \
        X(Pattern_Identifier,       Identifier)    \
        X(Pattern_Literal,          Literal)


    // Element patterns. These identify actual semantic elements in the code and can generate trees.
    #define LIST_PATTERN_ELM_TYPES_NAMES           \
        X(Pattern_Elm_Module,       Module)        \
        X(Pattern_Elm_Namespace,    Namespace)     \
        X(Pattern_Elm_Enum,         Enum)





    // Pattern struct signatures
    #define X(type, name) \
        struct type;
    LIST_PATTERN_OPERATOR_TYPES_NAMES
    LIST_PATTERN_BASES_TYPES_NAMES
    LIST_PATTERN_TOKENS_TYPES_NAMES
    LIST_PATTERN_ELM_TYPES_NAMES
    #undef X




    namespace re {
        // Custom sizeof function to use the size of the structs before they are formally defined
        template<class t> size_t forwardSizeof() {
            return sizeof(t);
        }


        // Pattern singletons and value generators
        // Usage: re::<name>(<patterns>)
        //! Placement new prevents circular dependencies between patterns
        #define X(type, name)                                                                   \
            extern type *__internal_singleton_##name;                                   \
            template<class ...t> type *name(t... subPatterns) {                                 \
                if(!__internal_singleton_##name) {                                      \
                    __internal_singleton_##name = reinterpret_cast<type*>(malloc(forwardSizeof<type>()));   cout << "allocated   " << __internal_singleton_##name << " | "#name << "\n";\
                    new (__internal_singleton_##name) type(subPatterns...);               cout << "initialized " << __internal_singleton_##name << " | "#name << "\n";\
                }                                                                               \
                else {\
                consoleLock.lock();                              \
                cout << "found       " << __internal_singleton_##name << " | "#name << "\n";\
                consoleLock.unlock();                              \
                }\
                return __internal_singleton_##name;                                     \
            }
        LIST_PATTERN_BASES_TYPES_NAMES
        LIST_PATTERN_ELM_TYPES_NAMES
        #undef X
    }
    // //BUG the keyword bug is prob caused by this generator function converting the Keyword ENUM value to a pointer and passing it to the constructor
    // //BUG tho there is no static cast here, idk





    namespace op {
        // Value generators for pattern operators (they don't need singletons and are stored in a different namespace)
        // Usage: op::<name>(<patterns>)
        #define X(type, name)                                   \
            template<class ...t> type *name(t... subPatterns) { \
                consoleLock.lock();                              \
                type* r = new type(subPatterns...); cout << "created     " << r << " | "#name << "\n";\
                consoleLock.unlock();                            \
                return r;                \
            }
        // LIST_PATTERN_OPERATOR_TYPES_NAMES
        X(__Pattern_Operator_Loop, Loop)
        #undef X
        //FIXME remove debug copy
        #define X(type, name)                                   \
            template<class ...t> type *name(t... subPatterns) { \
                consoleLock.lock();                              \
                type* r = new type(subPatterns...); cout << "created     " << r << " | "#name << "\n";\
                cout << "0: " << std::get<0>(std::make_tuple(subPatterns...)) << "\n";\
                cout << "1: " << std::get<1>(std::make_tuple(subPatterns...)) << "\n";\
                consoleLock.unlock();                            \
                return r;                \
            }
        // LIST_PATTERN_OPERATOR_TYPES_NAMES
        X(__Pattern_Operator_OneOf, OneOf)
        #undef X
    }



    namespace tk {
        // Value generators for token operators (they don't need singletons and are stored in a different namespace)
        // Usage: tk::<name>(<expected value>?)
        #define X(type, name)                                     \
            template<class ...t> type *name(t... expectedValue) { \
                consoleLock.lock();                              \
                type* r = new type(expectedValue...); cout << "created     " << r << " | "#name << "\n";\
                consoleLock.unlock();                            \
                return r;                \
            }
        LIST_PATTERN_TOKENS_TYPES_NAMES
        #undef X
    }



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
    };








    struct __base_Pattern_Token : public virtual __base_Pattern {
    };


    struct __base_Pattern_Composite : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;
        template<class ...t> __base_Pattern_Composite(t... _v) :
            v{ dynamic_cast<__base_Pattern*>(_v)... } { //FIXME this might need to be reverted to static_cast
        }

        virtual bool isChildAllowed(__base_ST* const child) const = 0;
        virtual __base_ST* generateData(std::vector<__base_ST*> const &parsedElements) const = 0;
    };








    struct __Pattern_Operator_Loop : public virtual __base_Pattern {
        __base_Pattern* v;
        __Pattern_Operator_Loop(__base_Pattern* _v) :
            v(_v) {
        }
    };

    struct __Pattern_Operator_OneOf : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;
        template<class ...t> __Pattern_Operator_OneOf(t... _v) :
            v{ dynamic_cast<__base_Pattern*>(_v)... } { //FIXME this might need to be reverted to static_cast
                //FIXME remove debug variables
            // __base_Pattern* _a           = reinterpret_cast<__base_Pattern*>(std::get<0>(std::make_tuple(_v...)));
            // __base_Pattern* _a_converted =     dynamic_cast<__base_Pattern*>(std::get<0>(std::make_tuple(_v...)));
            // __base_Pattern* _b           = reinterpret_cast<__base_Pattern*>(std::get<1>(std::make_tuple(_v...)));
            // __base_Pattern* _b_converted =     dynamic_cast<__base_Pattern*>(std::get<1>(std::make_tuple(_v...)));
            // auto test = dynamic_cast<__base_Pattern_Composite*>(_a_converted);
            // bool _a_converted_check = dynamic_cast<__base_Pattern_Composite*>(_a_converted)->isComposite();
            // bool _b_converted_check = dynamic_cast<__base_Pattern_Composite*>(_b_converted)->isComposite();
            // int dummy = 0;
        }
    };








    struct Pattern_Keyword : public virtual __base_Pattern_Token {
        ReservedTokenId id;
        Pattern_Keyword(ReservedTokenId _id) :
            id(_id) {
        }
    };

    struct Pattern_Identifier : public virtual __base_Pattern_Token {
        Pattern_Identifier(){}
    };

    struct Pattern_Literal : public virtual __base_Pattern_Token {
        Pattern_Literal(){}
    };





    //BUG nested namespaces don't seem they are being recognized
    //BUG nested namespaces don't seem they are being recognized
    //BUG nested namespaces don't seem they are being recognized



    struct Pattern_Elm_Namespace : public virtual __base_Pattern_Composite {
        Pattern_Elm_Namespace() : __base_Pattern_Composite(
            tk::Keyword(ReservedTokenId::KEYWORD_NAMESPACE),
            tk::Identifier(),
            tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
            //BUG turning on these lines makes the parser not work
            //BUG turning on these lines makes the parser not work
            //BUG turning on these lines makes the parser not work
            //BUG turning on these lines makes the parser not work
            op::Loop(op::OneOf(
                re::Enum(),
                re::Namespace()
                //FIXME other possible elements
            )),
            tk::Keyword(ReservedTokenId::KEYWORD_CURLY_R)
        ){}


        virtual bool isChildAllowed(__base_ST* const c) const {
            return !c->isStatement();
        }


        __base_ST* generateData(std::vector<__base_ST*> const &results) const override {
            ST_Namespace* r = new ST_Namespace;
            r->name = results[1]->asIdentifier();
            consoleLock.lock();                               //TODO remove
            cout << "found namespace " << r->name->s << "\n"; //TODO remove
            consoleLock.unlock();                             //TODO remove
            //TODO contents
            return r;
        }
    };



    //TODO set metakeyword string names to "meta keyword" (they are currently being referred to as "keywords")
    struct Pattern_Elm_Enum : public virtual __base_Pattern_Composite {
        Pattern_Elm_Enum() : __base_Pattern_Composite(
            tk::Keyword(ReservedTokenId::KEYWORD_ENUM),
            tk::Identifier(),
            tk::Keyword(ReservedTokenId::META_KEYWORD_BASE),
            tk::Identifier(),
            tk::Keyword(ReservedTokenId::KEYWORD_CURLY_L),
            op::Loop(op::OneOf(
                re::Enum(),
                re::Namespace()
                //FIXME other possible elements
            )),
            //BUG sub-element of the same type doesn't have the correct address
            //BUG the cached value is correct, but the value saved in the instance is not
            //! THIS DOES NOT DEPEND ON LOOP
            //! probably caused by OneOf or it might be a deeper problem
            new Pattern_Keyword(ReservedTokenId::KEYWORD_CURLY_R)
        ){}


        virtual bool isChildAllowed(__base_ST* const c) const {
            return !c->isStatement();
        }


        __base_ST* generateData(std::vector<__base_ST*> const &results) const override {
            ST_Enum* r = new ST_Enum;
            r->name = results[1]->asIdentifier();
            r->baseType = nullptr; //FIXME save actual type
            consoleLock.lock();                               //TODO remove
            cout << "found enum " << r->name->s << "\n";      //TODO remove
            consoleLock.unlock();                             //TODO remove
            //TODO contents
            return r;
        }
    };




    //TODO rename to "root"
    struct Pattern_Elm_Module : public virtual __base_Pattern_Composite {
        Pattern_Elm_Module() : __base_Pattern_Composite(
            // op::Loop(op::OneOf(
                // re::Namespace()
                re::Enum()
            // ))
        ){}


        virtual bool isChildAllowed(__base_ST* const c) const {
            return !c->isStatement();
        }


        __base_ST* generateData(std::vector<__base_ST*> const &results) const override {
            ST_Module* r = new ST_Module;
            for(ulong i = 0; i < results.size(); ++i) {
                r->addChild(results[i]);
            }
            return r;
        }
    };
}


//FIXME add "null" literal for pointers

//TODO ?        t (bool c) ? (t? value) { if(c) return $value; else return null; }
//TODO :        t (t? $a) : (t@ b) { if(a) return @a; else return b; }

//TODO [ takes a container and an index. it returns a pointer to the element that is at index <index>
//TODO ] takes a pointer and returns a reference to its value