#include "scope.hpp"
#include "Utils/errors.hpp"

#include "Compiler/TreePhase/Parsers/Elements/namespace.hpp"
#include "Compiler/TreePhase/Parsers/Elements/enum.hpp"
#include "Compiler/TreePhase/Parsers/Elements/struct.hpp"
#include "Compiler/TreePhase/Parsers/Elements/alias.hpp"
#include "Compiler/TreePhase/Parsers/Directives/import.hpp"
#include "Compiler/TreePhase/Parsers/Directives/export.hpp"








/**
 * @brief Parses all the elements of a scope
 * @param b  //TODO
 * @param index  //TODO
 * @param checkCurly  //TODO
 * @param scopeLen Where to save the length of the scope, measured in tokens. Can be null.
 * @return A vector containing the elements
 */
std::vector<cmp::__base_ST*> cmp::parseScope(TokenizedSource *b, ulong index, bool checkCurly, ulong* scopeLen) {
    std::vector<__base_ST*> r;
    ulong i = index;




    // Check { if needed
    std::optional<Token> const &t0 = (*b)[i];
    if(checkCurly) {
        if(!t0.has_value()) {
            utils::printError(
                ERROR_CMP_SCOPE_MISSING_INITIATOR,
                utils::ErrType::COMPILER,
                ElmCoords(b, i - 1, i - 1),
                "Expected a scope initiator ({), but the " + t0->genDecoratedValue() + " was found instead."
            );
        }
        else if(!t0->isKeyword(ReservedTokenId::KEYWORD_CURLY_L)) {
            utils::printError(
                ERROR_CMP_SCOPE_MISSING_INITIATOR,
                utils::ErrType::COMPILER,
                ElmCoords(b, i - 1, i - 1),
                "Missing scope initiator ({)"
            );
            // if(scopeLen) *scopeLen = 0;
            // return r;
        }
        ++i;
    }




    // Parse and save all the elements
    while((*b)[i].has_value() && !(*b)[i]->isKeyword(ReservedTokenId::KEYWORD_CURLY_R)) {
        std::optional<Token> const &t = (*b)[i];


        // Known declarations
        if(t->isKeyword()) {

            // Choose the parser function based on the keyword
            using parserFuncType = __base_ST* (*)(cmp::TokenizedSource*, ulong, ulong*);
            parserFuncType parserFunc;
            switch(t->getValue_Keyword()) {

                // Elements
                case ReservedTokenId::KEYWORD_NAMESPACE : { parserFunc = (parserFuncType)parseNamespace; break; }
                case ReservedTokenId::KEYWORD_ENUM      : { parserFunc = (parserFuncType)parseEnum;      break; }
                case ReservedTokenId::KEYWORD_STRUCT    : { parserFunc = (parserFuncType)parseStruct;    break; }
                case ReservedTokenId::KEYWORD_ALIAS     : { parserFunc = (parserFuncType)parseAlias;     break; }

                // Directives
                case ReservedTokenId::KEYWORD_IMPORT    : { parserFunc = (parserFuncType)parseImport;     break; }
                case ReservedTokenId::KEYWORD_EXPORT    : { parserFunc = (parserFuncType)parseExport;     break; }

                // Unexpected token
                default : {
                    utils::printError(
                        ERROR_CMP_UNEXPECTED_TOKEN,
                        utils::ErrType::COMPILER,
                        ElmCoords(b, i, i),
                        "Unexpected token \"" + (*b)[i]->OG_Value + "\"."
                    );
                }
            }

            // Call the parser function
            ++i; // Skip keyword //! Parsing starts from {
            ulong nodeLen;
            r.push_back(parserFunc(b, i, &nodeLen));
            i += nodeLen;
        }


        // Identifiers (functions, operators, variables)
        else if(t->isIdentifier()) {
            //! The identifier is not skipped as the parser needs to save it in the tree
            ulong nodeLen;
            r.push_back(nullptr); //FIXME actually parse it //FIXME pass nodeLen
            i += nodeLen;
        }


        // Unexpected tokens
        else {
            utils::printError(
                ERROR_CMP_UNEXPECTED_TOKEN,
                utils::ErrType::COMPILER,
                ElmCoords(b, i, i),
                "Unexpected token \"" + (*b)[i]->OG_Value + "\"."
            );
        }
    }




    // Check } if needed
    std::optional<Token> const &t1 = (*b)[i];
    if(checkCurly){
        if(!t1.has_value()) {
            utils::printError(
                ERROR_CMP_SCOPE_MISSING_TERMINATOR,
                utils::ErrType::COMPILER,
                ElmCoords(b, i - !t1.has_value(), i - !t1.has_value()),
                "Missing scope terminator (}) after scope definition."
            );
        }
        ++i;
    }




    // Set the scope length and return the elements
    if(scopeLen) *scopeLen = i - index;
    return r;
}