#include "import.hpp"




cmp::ST_Import* cmp::parseImport(TokenizedSource* b, ulong index) {
    ST_Import* r = new ST_Import();
    ulong i = index;

    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}