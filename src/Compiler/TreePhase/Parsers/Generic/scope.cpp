// #include "scope.hpp"
// #include "Utils/errors.hpp"

// #include "Compiler/TreePhase/Parsers/Elements/namespace.hpp"
// #include "Compiler/TreePhase/Parsers/Elements/enum.hpp"
// #include "Compiler/TreePhase/Parsers/Elements/struct.hpp"
// #include "Compiler/TreePhase/Parsers/Elements/alias.hpp"
// #include "Compiler/TreePhase/Parsers/Directives/import.hpp"
// #include "Compiler/TreePhase/Parsers/Directives/export.hpp"








// /**
//  * @brief Parses all the elements of a scope
//  * @param b  //TODO
//  * @param index  //TODO
//  * @param checkCurly  //TODO
//  * @param scopeLen Where to save the length of the scope, measured in tokens. Can be null.
//  * @return A vector containing the elements
//  */
// std::vector<cmp::__base_ST*> cmp::generic_parseScope(TokenizedSource *b, ulong index, bool checkCurly, ulong *scopeLen) {
//     std::vector<__base_ST*> r;
//     ulong i = index;




//     // Check { if needed
//     std::optional<Token> const &t0 = (*b)[i];
//     if(checkCurly) {
//         if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_CURLY_L)) {
//             utils::printError(
//                 ERROR_CMP_SCOPE_MISSING_INITIATOR, utils::ErrType::COMPILER,
//                 ElmCoords(b, i, i), //TODO maybe pass the relevant section from the caller functio
//                 (t0.has_value() ? "Expected a scope initiator ({), but the " + t0->genDecoratedValue() + " was found instead." : "Missing scope initiator ({)")
//             );
//         }
//         ++i;
//     }




//     // Parse and save all the elements
//     while((*b)[i].has_value() && !(*b)[i]->isKeyword(ReservedTokenId::KEYWORD_CURLY_R)) {
//         std::optional<Token> const &t = (*b)[i];


//         // Parse element
//         __base_ST*  result = parseNamespace(b, i);
//         if(!result) result = parseEnum     (b, i);
//         if(!result) result = parseStruct   (b, i);
//         if(!result) result = parseAlias    (b, i);
//         if(!result) result = parseImport   (b, i);
//         if(!result) result = parseExport   (b, i);
//         if(result) {
//             r.push_back(result);
//             i = result->tokenEnd + 1;
//         }


//         // Unexpected tokens
//         else {
//             utils::printError(
//                 ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
//                 ElmCoords(b, i, i),
//                 "Unexpected " + t->genDecoratedValue() + "."
//             );
//         }
//     }




//     // Check } if needed
//     std::optional<Token> const &t1 = (*b)[i];
//     if(checkCurly){
//         if(!t1.has_value()) {
//             utils::printError(
//                 ERROR_CMP_SCOPE_MISSING_TERMINATOR, utils::ErrType::COMPILER,
//                 ElmCoords(b, index, i - 1),
//                 ElmCoords(b, i - 1, i - 1),
//                 "Missing scope terminator (}) after scope definition."
//             );
//         }
//         //! Checking that the token is not a } isn't needed as the
//         //! previous loop has this requirement as part of its condition
//         ++i;
//     }




//     // Set the scope length and return the elements
//     if(scopeLen) *scopeLen = i - index;
//     return r;
// }