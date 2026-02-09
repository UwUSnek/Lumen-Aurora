#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "__base_Pattern_Token.hpp"




namespace cmp {
    // Wrapper for identifier tokens
    struct ST_Sub_Identifier : public virtual __base_ST {
        std::string s;
        std::string getCategoryName(bool plural = false) const override;

        explicit ST_Sub_Identifier(const std::string &_s) :
            s(_s) {
        }
    };




    struct Pattern_Identifier : public virtual __base_Pattern_Token {
        void init() const{
            // Empty
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}