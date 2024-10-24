#pragma once
#include "__base_Pattern_Operator.hpp"




namespace cmp {
    struct __Pattern_Operator_Optional : public virtual __base_Pattern_Operator {
        ulong threshold;
        template<class ...t> void init(t... _v) {
            __internal_init_Optional<t...>(_v...);
        }
        template<class _thresholdTypeDiscard, class ...t> void __internal_init_Optional(ulong _threshold, t... _v) {
            __base_Pattern_Operator::__internal_init(_v...);
            threshold = _threshold;
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}