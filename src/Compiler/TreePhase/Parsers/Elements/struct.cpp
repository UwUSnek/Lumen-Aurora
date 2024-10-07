#include "struct.hpp"




cmp::ST_Struct* cmp::parseStruct(TokenizedSource* b, ulong index) {
    ST_Struct* r = new ST_Struct();
    ulong i = index;

    // Set the element length and return the node
    r->tokenBgn = index - 1; //! Account for the struct keyword
    r->tokenEnd = i - 1;
    return r;
}