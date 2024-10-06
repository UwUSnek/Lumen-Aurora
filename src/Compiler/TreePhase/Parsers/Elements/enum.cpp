#include "enum.hpp"




cmp::ST_Enum* cmp::parseEnum(TokenizedSource* b, ulong index) {
    ST_Enum* r = new ST_Enum();
    ulong i = index;

    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}