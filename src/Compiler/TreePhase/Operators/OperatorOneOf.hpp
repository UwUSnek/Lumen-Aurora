#pragma once
#include "__base_Pattern_Operator.hpp"




namespace cmp {
    struct __Pattern_Operator_OneOf : public virtual __base_Pattern_Operator {
        template<class ...t> void init(t... _v) {
            __base_Pattern_Operator::__internal_init(_v...);
            debug(for(auto p : v) if(p->isOptional()) (cout++ << "Optional operator used as direct child of OneOf. This is not allowed.")--;)
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}