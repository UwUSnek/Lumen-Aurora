#include "DoubleLiteral.hpp"




std::string cmp::Pattern_DoubleLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Double Literal"
        :   "Double Literal"
    ;
}

ulong cmp::Pattern_DoubleLiteral::getCertaintyThreshold() const {
    return 1;
}