// #include "export.hpp"




// cmp::ST_Export* cmp::parseExport(TokenizedSource* b, ulong index) {
//     std::optional<Token> const &t0 = (*b)[index];
//     if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_EXPORT)) {
//         return nullptr;
//     }
//     ST_Export* r = new ST_Export();
//     ulong i = index + 1;

//     // Set the element length and return the node
//     r->tokenBgn = index;
//     r->tokenEnd = i - 1;
//     return r;
// }