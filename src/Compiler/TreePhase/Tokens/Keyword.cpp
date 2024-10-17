#include "Keyword.hpp"




std::string cmp::Pattern_Keyword::genDecoratedValue() const {
    return "The keyword \"" + std::find_if(
        reservedTokensMap.begin(),
        reservedTokensMap.end(),
        [&](const auto& pair) { return pair.second == id; }
    )->first + "\"";
}




ulong cmp::Pattern_Keyword::getCertaintyThreshold() const {
    return 1;
}