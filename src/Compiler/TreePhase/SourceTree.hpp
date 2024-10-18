#pragma once
#include <vector>
#include <string>

#include "ALC.hpp"
#include "Utils/Containers/GenericPipe.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"








namespace cmp {

    #define LIST_SOURCE_TREE_TYPES_NAMES    \
        X(ST_Sub_Identifier, Identifier)    \
        X(ST_Sub_Keyword,    Keyword)    \
        /* X(ST_Sub_Literal, Identifier)    //TODO*/\
        \
        X(ST_Sub_Path,       Path)           \
        X(ST_BasicType,      BasicType)      \
        X(ST_FunctionType,   FunctionType)   \
        X(ST_Type,           Type)           \
        \
        X(ST_Statement,      Statement)      \
        \
        X(ST_Module,         Module)         \
        X(ST_Namespace,      Namespace)      \
        X(ST_Enum,           Enum)           \
        X(ST_EnumElm,        EnumElm)        \
        X(ST_Struct,         Struct)         \
        X(ST_Alias,          Alias)




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


    // // Reflection path expression
    // struct ST_Expr_Reflection : public virtual __base_ST {
    //     ST_Sub_Path* symbolPath;
    //     ST_Sub_Path* reflectionIdList;
    // }; //FIXME parse this in the generic scope parser

    //TODO
    //TODO
    //TODO




    // Base struct for any semantic element that can be referenced by its name
    struct __base_ST_Referable : public virtual __base_ST {
        ST_Sub_Identifier *name;
    };


    // Base struct for any semantic element that has an explicit type
    struct __base_ST_Typed : public virtual __base_ST {
        ST_Type *type;
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
}