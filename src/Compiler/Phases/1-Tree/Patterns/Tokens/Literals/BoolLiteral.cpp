#include "BoolLiteral.hpp"




std::string cmp::Pattern_BoolLiteral::genDecoratedValue(bool article) const {
    return article
        ? "a Boolean Literal"
        :   "Boolean Literal"
    ;
}

ulong cmp::Pattern_BoolLiteral::getCertaintyThreshold() const {
    return 1;
}