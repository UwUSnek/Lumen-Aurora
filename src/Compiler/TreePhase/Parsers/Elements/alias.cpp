#include "alias.hpp"
#include "Utils/errors.hpp"
#include "Compiler/TreePhase/Parsers/SubElements/path.hpp"








cmp::ST_Alias* cmp::parseAlias(TokenizedSource* b, ulong index) {
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_ALIAS)) {
        return nullptr;
    }
    ST_Alias* r = new ST_Alias();
    ulong i = index + 1;




    // Parse original path
    ST_Sub_Path* original = parsePath(b, i);
    if(!original) {
        if(!(*b)[i].has_value()) {
            utils::printError(
                ERROR_CMP_ALIAS_NO_PATH,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "Incomplete definition of Symbol alias.\n"
                "The symbol path is missing."
            );
        }
        else {
            utils::printError(
                ERROR_CMP_ALIAS_NO_PATH,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i),
                ElmCoords(b, i,     i),
                "Expected a symbol path, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
            );
        }
    }
    r->original = original;
    i = original->tokenEnd + 1;




    // Parse "as" keyword
    std::optional<Token> const &t2 = (*b)[i];
    if(!t2.has_value()){
        utils::printError(
            ERROR_CMP_ALIAS_NO_AS,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            ElmCoords(b, i - 1, i - 1),
            "Incomplete definition of Symbol alias.\n"
            "The \"as\" keyword is missing."
        );
    }
    else if(!t2->isKeyword(ReservedTokenId::META_KEYWORD_AS)) {
        utils::printError(
            ERROR_CMP_ALIAS_NO_AS,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Expected the keyword \"as\", but the " + t2->genDecoratedValue() + " was found instead."
        );
    }
    ++i;




    // Parse new name
    std::optional<Token> const &t3 = (*b)[i];
    if(!t3.has_value()) {
        utils::printError(
            ERROR_CMP_ALIAS_NO_NAME,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            ElmCoords(b, i - 1, i - 1),
            "Incomplete definition of Symbol alias.\n"
            "The identifier is missing."
        );
    }
    else if(!t3->isIdentifier()) {
        utils::printError(
            ERROR_CMP_ALIAS_NO_NAME,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Expected an identifier, but the " + t3->genDecoratedValue() + " was found instead."
        );
    }
    r->name = t3->getValue_Identifier();
    ++i;




    // Check ;
    std::optional<Token> const &t4 = (*b)[i];
    if(!t4.has_value() || !t4->isKeyword(ReservedTokenId::KEYWORD_SEMICOLON)) {
        utils::printError(
            ERROR_CMP_ALIAS_NO_NAME,
            utils::ErrType::COMPILER,
            ElmCoords(b, index,               i - !t4.has_value()),
            ElmCoords(b, i - !t4.has_value(), i - !t4.has_value()),
            "Missing semicolon (;) after Symbol Alias definition."
        );
    }
    ++i;




    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}