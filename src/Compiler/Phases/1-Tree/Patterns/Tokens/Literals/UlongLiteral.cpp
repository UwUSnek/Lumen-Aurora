#include "UlongLiteral.hpp"




std::string cmp::Pattern_UlongLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a uLong Literal"
        :   "uLong Literal"
    ;
}

ulong cmp::Pattern_UlongLiteral::getCertaintyThreshold() const {
    return 1;
}