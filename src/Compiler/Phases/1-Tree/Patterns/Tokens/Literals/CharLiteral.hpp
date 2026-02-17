#pragma once
#include "Compiler/Phases/1-Tree/Patterns/Tokens/__base_Pattern_Token.hpp"
#include "Compiler/Phases/1-Tree/SourceTree.hpp"




namespace cmp {
    // Wrapper for char literal tokens
    struct ST_Sub_CharLiteral : public virtual __base_ST {
        char value;
        std::string getCategoryName(bool plural = false) const override;

        explicit ST_Sub_CharLiteral(const char _value) :
            value(_value) {
        }
    };




    struct Pattern_CharLiteral : public virtual __base_Pattern_Token {
        void init() const {
            //Empty
        }

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}