#include "Literal.hpp"




std::string cmp::Pattern_Literal::genDecoratedValue(bool article) const {
    return article
        ? "a Literal"
        :   "Literal"
    ;
}

ulong   cmp::Pattern_Literal::getCertaintyThreshold() const {
    return 1;
}