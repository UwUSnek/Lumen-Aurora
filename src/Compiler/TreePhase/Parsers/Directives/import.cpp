// #include "import.hpp"




// cmp::ST_Import* cmp::parseImport(TokenizedSource* b, ulong index) {
//     std::optional<Token> const &t0 = (*b)[index];
//     if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_IMPORT)) {
//         return nullptr;
//     }
//     ST_Import* r = new ST_Import();
//     ulong i = index + 1;

//     // Set the element length and return the node
//     r->tokenBgn = index;
//     r->tokenEnd = i - 1;
//     return r;
// }