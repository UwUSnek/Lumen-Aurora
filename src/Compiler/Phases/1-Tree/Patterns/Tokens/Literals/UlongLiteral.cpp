#include "UlongLiteral.hpp"

std::string cmp::ST_Sub_UlongLiteral::getCategoryName(bool plural) const {
    return plural ? "ulong literals" : "ulong literal";
}




std::string cmp::Pattern_UlongLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a uLong Literal"
        :   "uLong Literal"
    ;
}

ulong cmp::Pattern_UlongLiteral::getCertaintyThreshold() const {
    return 1;
}