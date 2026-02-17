#include "StrLiteral.hpp"




std::string cmp::Pattern_StrLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Str Literal"
        :   "Str Literal"
    ;
}

ulong cmp::Pattern_StrLiteral::getCertaintyThreshold() const {
    return 1;
}