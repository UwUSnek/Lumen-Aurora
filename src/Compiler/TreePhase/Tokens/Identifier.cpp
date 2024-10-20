#include "Identifier.hpp"




std::string cmp::Pattern_Identifier::genDecoratedValue(bool article) const {
    return article
        ? "an Identifier"
        :    "Identifier"
    ;
}

ulong   cmp::Pattern_Identifier::getCertaintyThreshold() const {
    return 1;
}
