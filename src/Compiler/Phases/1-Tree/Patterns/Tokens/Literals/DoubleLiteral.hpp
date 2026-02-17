#pragma once
#include "Compiler/Phases/1-Tree/Patterns/Tokens/__base_Pattern_Token.hpp"
#include "Compiler/Phases/1-Tree/SourceTree.hpp"




namespace cmp {
    // Wrapper for double literal tokens
    struct ST_Sub_DoubleLiteral : public virtual __base_ST {
        double value;
        std::string getCategoryName(bool plural = false) const override;

        explicit ST_Sub_DoubleLiteral(const double _value) :
            value(_value) {
        }
    };




    struct Pattern_DoubleLiteral : public virtual __base_Pattern_Token {
        void init() const {
            //Empty
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}