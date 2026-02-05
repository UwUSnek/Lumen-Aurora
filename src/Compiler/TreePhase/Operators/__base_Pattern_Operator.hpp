#pragma once
#include "Compiler/TreePhase/__base_Pattern.hpp"
#include <vector>




namespace cmp {
    struct __base_Pattern_Operator : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;
        __base_Pattern *parent;

        void initParentReferences();
        template<class ...t> void __internal_init(t... _v) {
            v = std::vector<__base_Pattern*>{ dynamic_cast<__base_Pattern*>(_v)... };
            initParentReferences();
        }
    };
}