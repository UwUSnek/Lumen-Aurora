#include "Keyword.hpp"




std::string cmp::Pattern_Keyword::genDecoratedValue(bool article) const {
    if(article) {
        for(const auto& [key, tokenId] : reservedTokensMap) {
            if(tokenId == id) return "the Keyword \"" + key + "\"";
        }
    }
    else {
        return "a Keyword";
    }

    //! Bogus return value for GCC
    return "";
}
ulong cmp::Pattern_Keyword::getCertaintyThreshold() const { return 1; }