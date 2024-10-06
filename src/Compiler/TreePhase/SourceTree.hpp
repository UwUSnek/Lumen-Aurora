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
        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~__base_ST(){}
        virtual bool isChildAllowed(__base_ST* c) const;


        bool isType     () const;
        bool isPath     () const;

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
        bool isChildAllowed(__base_ST* c) const override;
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








    // Base struct for every first-pass semantic element
    struct __base_ST_Container : public virtual __base_ST {
        std::vector<__base_ST*> children;

        void addChild(__base_ST* c) {
            children.push_back(c);
        }
    };


    // Base struct for any semantic element that can be referenced by its name
    struct __base_ST_Referable : public virtual __base_ST {
        std::string name;
    };


    struct ST_Sub_Path;
    // Base struct for any semantic element that has an explicit type
    struct __base_ST_Typed : public virtual __base_ST {
        ST_Sub_Type *type;
    };








    // Complex symbol path sub-element (identifiers separated by a dot keyword)
    struct ST_Sub_Path : public virtual __base_ST {
        std::vector<std::string> idList;
        bool isChildAllowed(__base_ST* c) const override;
    };

    // // Expressions
    // //! The array of children contains the sub-expressions
    // struct ST_Expr : public virtual __base_ST_Container {
    //     bool isChildAllowed(__base_ST* c) const override;
    // };
    //FIXME basic expression types








    struct ST_Module : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //TODO set the name in the treePhase parsing function
        //TODO take an alternative name from a parameter (for "import as"). in this case, the file's name can be invalid
        //TODO If not imported "as", the file name must be a valid identifier (after removing any extension)
        bool isChildAllowed(__base_ST* c) const override;
    };

    struct ST_Namespace : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        bool isChildAllowed(__base_ST* c) const override;
    };

    struct ST_Struct : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //FIXME struct members
        bool isChildAllowed(__base_ST* c) const override;
    };

    struct ST_Enum : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //FIXME base type
        //FIXME enum members
        bool isChildAllowed(__base_ST* c) const override;
    };

    struct ST_Alias : public virtual __base_ST_Referable {
        ST_Sub_Path *original = nullptr;
        bool isChildAllowed(__base_ST* c) const override;
    };








    struct ST_Import : public virtual __base_ST {
        //FIXME whatever import needs
        bool isChildAllowed(__base_ST* c) const override;
    };
    struct ST_Export : public virtual __base_ST {
        //FIXME whatever export needs
        bool isChildAllowed(__base_ST* c) const override;
    };










    //! Expressions are saved in the generic child array, together with the other elements
    struct __base_ST_Routine : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        //FIXME parameters
        //FIXME specialization constraint

        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~__base_ST_Routine(){}
    };

    struct ST_Variable : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        //FIXME default value
        //FIXME const qualifier
        bool isChildAllowed(__base_ST* c) const override;
    };
    struct ST_Function : public virtual __base_ST_Routine {
        bool isChildAllowed(__base_ST* c) const override;
    };
    struct ST_Operator : public virtual __base_ST_Routine {
        //FIXME multiple names (or name position in case of split ternary)
        //FIXME priority
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