#include "Identifier.hpp"

std::string cmp::ST_Sub_Identifier::getCategoryName(bool plural) const {
    return plural ? "identifiers"  : "identifier";
}




std::string cmp::Pattern_Identifier::genDecoratedValue(bool article) const {
    return article
        ? "an Identifier"
        :    "Identifier"
    ;
}

ulong cmp::Pattern_Identifier::getCertaintyThreshold() const {
    return 1;
}
