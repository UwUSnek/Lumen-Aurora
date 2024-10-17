#pragma once
#include "__base_Pattern_Token.hpp"




namespace cmp {
    struct Pattern_Identifier : public virtual __base_Pattern_Token {
        void init(){}

        std::string genDecoratedValue() const override;
        ulong getCertaintyThreshold() const override;
    };
}