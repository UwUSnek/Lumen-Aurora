#include "path.hpp"
#include "Utils/errors.hpp"

//FIXME free the tokens after using them
//FIXME also free the other stuff




cmp::ST_Sub_Path* cmp::parsePath(TokenizedSource* b, ulong index) {

    // Save the starting identifier
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isIdentifier()) {
        return nullptr;
    }
    ST_Sub_Path *r = new ST_Sub_Path();
    ulong i = index + 1;
    r->idList.push_back(t0->getValue_Identifier());




    // Save subsequent separator-identifier pairs
    while(true) {


        // Skip . separator, return if one is not found
        std::optional<Token> const &ta = (*b)[i];
        if(!ta.has_value() || !ta->isKeyword(ReservedTokenId::KEYWORD_DOT)) {
            break;
        }
        ++i;


        // Save identifier, print an error if one is not found
        std::optional<Token> const &tb = (*b)[i];
        if(!tb.has_value()) {
            utils::printError(
                ERROR_CMP_PATH_NO_IDENTIFIER,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "Incomplete symbol path.\n"
                "The identifier is missing."
            );
        }
        else if(!tb->isIdentifier()) {
            utils::printError(
                ERROR_CMP_PATH_NO_IDENTIFIER,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i),
                ElmCoords(b, i,     i),
                "Expected an identifier, but the " + tb->genDecoratedValue() + " was found instead."
            );
        }
        r->idList.push_back(tb->getValue_Identifier());
        ++i;
    }




    // Set element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}