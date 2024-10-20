#include "Keyword.hpp"




std::string cmp::Pattern_Keyword::genDecoratedValue() const {
    for (const auto& pair : reservedTokensMap) {
        if(pair.second == id) return "The keyword \"" + pair.first + "\"";
    }

    //! Bogus return value for GCC
    return "";
}
ulong cmp::Pattern_Keyword::getCertaintyThreshold() const { return 1; }