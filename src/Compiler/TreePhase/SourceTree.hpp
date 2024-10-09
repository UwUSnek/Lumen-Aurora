#pragma once
#include <vector>
#include <string>

#include "Utils/Containers/GenericPipe.hpp"








namespace cmp {
    struct ST_Module;
    /**
     * @brief The base Source Tree structure
     */
    struct __base_ST {
        ulong tokenBgn;     // The index of the token in which this element begins
        ulong tokenEnd;     // The index of the token in which this element ends
        __base_ST* parent; //TODO set module's parent
        //TODO also set the root module's parent to NULL

        virtual bool isChildAllowed(__base_ST* c)                const { return false; }
        virtual std::string getCategoryName(bool plural = false) const { return ""; }

        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~__base_ST() = default;


        bool isType     () const;
        bool isPath     () const;

        bool isStatement() const;

        // bool isExpr     () const;

        bool isModule   () const;
        bool isNamespace() const;
        bool isStruct   () const;
        bool isEnum     () const;
        bool isAlias    () const;

        bool isImport   () const;
        bool isExport   () const;

        bool isVariable () const;
        bool isFunction () const;
        bool isOperator () const;
    };

    typedef GenericPipe<ST_Module> SourceTree;







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
        std::string name;
    };


    struct ST_Sub_Path;
    // Base struct for any semantic element that has an explicit type
    struct __base_ST_Typed : public virtual __base_ST {
        ST_Sub_Type *type;
    };








    // Statements
    struct ST_Statement : public virtual __base_ST {
        bool isChildAllowed(__base_ST* c) const override;
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
        bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };

    struct ST_Namespace : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };

    struct ST_Struct : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //FIXME struct members
        bool isChildAllowed(__base_ST* c) const override;
        std::string getCategoryName(bool plural = false) const override;
    };

    struct ST_Enum : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        ST_Sub_Path *baseType; //FIXME use type path
        //! Enum elements are saved in the children vector

        bool isChildAllowed(__base_ST* c) const override;
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
        bool isChildAllowed(__base_ST* c) const override;
    };
    struct ST_Operator : public virtual __base_ST_Routine {
        //FIXME multiple names (or name position in case of split ternary)
        //FIXME priority
        std::string getCategoryName(bool plural = false) const override;
        bool isChildAllowed(__base_ST* c) const override;

        //FIXME make [] use the normal syntax
    };
}

//TODO add all the .isSomething functions
//TODO add all the .getAs_Something functions




//TODO ? returns a pointer. it points to the 2nd parameter if the first evaluates to true, or NULL if it evaluates to false
//TODO : takes a pointer and a value. it returns the value pointer by the pointer if it's a valid address, or the second value if it's NULL.

//TODO [ takes a container and an index. it returns a pointer to the element that is at index <index>
//TODO ] takes a pointer and returns a reference to its value