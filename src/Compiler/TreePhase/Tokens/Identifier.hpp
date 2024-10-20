#pragma once
#include "__base_Pattern_Token.hpp"




namespace cmp {
    // Wrapper for identifier tokens
    struct ST_Sub_Identifier : public virtual __base_ST {
        std::string s;
        std::string getCategoryName(bool plural = false) const override;

        ST_Sub_Identifier(std::string _s) :
            s(_s) {
        }
    };




    struct Pattern_Identifier : public virtual __base_Pattern_Token {
        void init(){}

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}