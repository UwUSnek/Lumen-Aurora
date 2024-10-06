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
    };
    typedef GenericPipe<ST_Module> SourceTree;








    struct __base_ST_Container : public virtual __base_ST {
        std::vector<__base_ST*> children;

        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~__base_ST_Container(){}

        void addChild(__base_ST* c) {
            children.push_back(c);
        }
    };

    struct __base_ST_Referable : public virtual __base_ST {
        std::string name;

        // Make the destructor virtual so that dynamic_cast sees TokenValue* as polymorphic
        virtual ~__base_ST_Referable(){}
    };








    struct ST_Path : public virtual __base_ST {
        std::vector<std::string> idList;
    };








    struct ST_Module : public virtual __base_ST_Referable, public virtual __base_ST_Container {
    };

    struct ST_Namespace : public virtual __base_ST_Referable, public virtual __base_ST_Container {
    };

    struct ST_Struct : public virtual __base_ST_Referable, public virtual __base_ST_Container {
    };

    struct ST_Enum : public virtual __base_ST_Referable, public virtual __base_ST_Container {
    };

    struct ST_Alias : public virtual __base_ST_Referable {
        ST_Path *original = nullptr;
    };








    struct ST_Import : public virtual __base_ST {
    };
    struct ST_Export : public virtual __base_ST {
    };
}

//TODO add all the .isSomething functions
//TODO add all the .getAs_Something functions