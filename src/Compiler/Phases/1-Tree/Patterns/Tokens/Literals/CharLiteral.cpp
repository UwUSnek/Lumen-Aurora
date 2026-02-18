#include "CharLiteral.hpp"

std::string cmp::ST_Sub_CharLiteral::getCategoryName(bool plural) const {
    return plural ? "char literals" : "char literal";
}




std::string cmp::Pattern_CharLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Char Literal"
        :   "Char Literal"
    ;
}

ulong cmp::Pattern_CharLiteral::getCertaintyThreshold() const {
    return 1;
}