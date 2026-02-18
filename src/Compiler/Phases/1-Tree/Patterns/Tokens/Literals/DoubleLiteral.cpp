#include "DoubleLiteral.hpp"

std::string cmp::ST_Sub_DoubleLiteral::getCategoryName(bool plural) const {
    return plural ? "double literals" : "double literal";
}




std::string cmp::Pattern_DoubleLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Double Literal"
        :   "Double Literal"
    ;
}

ulong cmp::Pattern_DoubleLiteral::getCertaintyThreshold() const {
    return 1;
}