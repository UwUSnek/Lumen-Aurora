// #include "struct.hpp"
// #include "Utils/errors.hpp"








// cmp::ST_Struct* cmp::parseStruct(TokenizedSource* b, ulong index) {
//     std::optional<Token> const &t0 = (*b)[index];
//     if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_STRUCT)) {
//         return nullptr;
//     }
//     ST_Struct* r = new ST_Struct();
//     ulong i = index;

//     // Set the element length and return the node
//     r->tokenBgn = index;
//     r->tokenEnd = i - 1;
//     return r;
// }