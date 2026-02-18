#include "BoolLiteral.hpp"

std::string cmp::ST_Sub_BoolLiteral::getCategoryName(bool plural) const {
    return plural ? "boolean literals" : "boolean literal";
}




std::string cmp::Pattern_BoolLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Boolean Literal"
        :   "Boolean Literal"
    ;
}

ulong cmp::Pattern_BoolLiteral::getCertaintyThreshold() const {
    return 1;
}