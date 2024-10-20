#pragma once
#include "__base_Pattern_Token.hpp"




namespace cmp {
    struct Pattern_Literal : public virtual __base_Pattern_Token {
        void init(){}

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}