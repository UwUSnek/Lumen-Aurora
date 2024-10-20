#include "Keyword.hpp"




std::string cmp::Pattern_Keyword::genDecoratedValue(bool article) const {
    if(article) {
        for (const auto& pair : reservedTokensMap) {
            if(pair.second == id) return "the Keyword \"" + pair.first + "\"";
        }
    }
    else {
        return "a Keyword";
    }

    //! Bogus return value for GCC
    return "";
}
ulong cmp::Pattern_Keyword::getCertaintyThreshold() const { return 1; }