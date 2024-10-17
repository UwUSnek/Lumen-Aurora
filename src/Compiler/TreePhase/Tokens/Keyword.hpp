#pragma once
#include "__base_Pattern_Token.hpp"




namespace cmp {
    struct Pattern_Keyword : public virtual __base_Pattern_Token {
        ReservedTokenId id;
        void init(ReservedTokenId _id){
            id = _id;
        }

        std::string genDecoratedValue() const override;
        ulong getCertaintyThreshold() const override;
    };
}