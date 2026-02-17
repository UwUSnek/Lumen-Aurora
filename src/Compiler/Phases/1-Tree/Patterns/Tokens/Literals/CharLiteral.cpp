#include "CharLiteral.hpp"




std::string cmp::Pattern_CharLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Char Literal"
        :   "Char Literal"
    ;
}

ulong cmp::Pattern_CharLiteral::getCertaintyThreshold() const {
    return 1;
}