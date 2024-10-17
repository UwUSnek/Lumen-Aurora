#pragma once
#include "__base_Pattern_Token.hpp"




namespace cmp {
    // Wrapper for keyword tokens
    struct ST_Sub_Keyword : public virtual __base_ST {
        ReservedTokenId id;
        std::string getCategoryName(bool plural = false) const override;

        ST_Sub_Keyword(ReservedTokenId _id) :
            id(_id) {
        }
    };




    struct Pattern_Keyword : public virtual __base_Pattern_Token {
        ReservedTokenId id;
        void init(ReservedTokenId _id){
            id = _id;
        }

        std::string genDecoratedValue() const override;
        ulong getCertaintyThreshold() const override;
    };
}