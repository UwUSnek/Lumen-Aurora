#pragma once
#include "Compiler/Phases/1-Tree/Patterns/Tokens/__base_Pattern_Token.hpp"
#include "Compiler/Phases/1-Tree/SourceTree.hpp"




namespace cmp {
    // Wrapper for bool literal tokens
    struct ST_Sub_BoolLiteral : public virtual __base_ST {
        bool value;
        std::string getCategoryName(bool plural = false) const override;

        explicit ST_Sub_BoolLiteral(const bool _value) :
            value(_value) {
        }
    };




    struct Pattern_BoolLiteral : public virtual __base_Pattern_Token {
        void init() const {
            //Empty
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}