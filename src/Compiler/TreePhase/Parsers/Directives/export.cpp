#include "export.hpp"




cmp::ST_Export* cmp::parseExport(TokenizedSource* b, ulong index) {
    ST_Export* r = new ST_Export();
    ulong i = index;

    // Set the element length and return the node
    r->tokenBgn = index - 1; //! Account for the export keyword
    r->tokenEnd = i - 1;
    return r;
}