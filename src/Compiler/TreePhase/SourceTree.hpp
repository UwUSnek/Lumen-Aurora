#pragma once
#include <vector>
#include <string>








namespace cmp {
    /**
     * @brief The base Source Tree structure
     */
    struct __base_ST {
        // Empty
    };








    struct __base_ST_Container : public virtual __base_ST {
        std::vector<__base_ST*> children;
    };

    struct __base_ST_Referable : public virtual __base_ST {
        std::string name;
    };








    struct ST_Module : public __base_ST_Referable, public __base_ST_Container {
    };

    struct ST_Namespace : public __base_ST_Referable, public __base_ST_Container {
    };

    struct ST_Struct : public __base_ST_Referable, public __base_ST_Container {
    };
}