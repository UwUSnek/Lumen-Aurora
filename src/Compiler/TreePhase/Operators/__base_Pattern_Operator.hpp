#pragma once
#include "Compiler/TreePhase/__base_Pattern.hpp"




namespace cmp {
    struct __base_Pattern_Operator : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;

        template<class ...t> void __internal_init(t... _v) {
            v = std::vector<__base_Pattern*>{ dynamic_cast<__base_Pattern*>(_v)... };
        }
    };
}