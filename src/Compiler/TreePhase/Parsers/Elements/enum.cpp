#include "enum.hpp"




cmp::ST_Enum* cmp::parseEnum(TokenizedSource* b, ulong index) {
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_ENUM)) {
        return nullptr;
    }
    ST_Enum* r = new ST_Enum();
    ulong i = index + 1;




    //




    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}