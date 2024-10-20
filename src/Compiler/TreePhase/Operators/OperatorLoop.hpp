#pragma once
#include "__base_Pattern_Operator.hpp"




namespace cmp {
    struct __Pattern_Operator_Loop : public virtual __base_Pattern_Operator {
        template<class ...t> void init(t... _v) {
            __base_Pattern_Operator::__internal_init(_v...);
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}