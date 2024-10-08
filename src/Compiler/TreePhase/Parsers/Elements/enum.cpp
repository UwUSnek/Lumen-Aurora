#include "enum.hpp"
#include "Utils/errors.hpp"








cmp::ST_Enum* cmp::parseEnum(TokenizedSource* b, ulong index) {
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isKeyword(ReservedTokenId::KEYWORD_ENUM)) {
        return nullptr;
    }
    ST_Enum* r = new ST_Enum();
    ulong i = index + 1;




    // Parse name
    std::optional<Token> const &t1 = (*b)[i];
    if(!t1.has_value()) {
        utils::printError(
            ERROR_CMP_ALIAS_NO_NAME, utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            ElmCoords(b, i - 1, i - 1),
            "Incomplete definition of Enum.\n"
            "The identifier is missing."
        );
    }
    else if(!t1->isIdentifier()) {
        utils::printError(
            ERROR_CMP_ALIAS_NO_NAME, utils::ErrType::COMPILER,
            ElmCoords(b, index, i),
            ElmCoords(b, i,     i),
            "Incomplete definition of Enum.\n"
            "Expected an identifier, but the " + t1->genDecoratedValue() + " was found instead."
        );
    }
    r->name = t1->getValue_Identifier();
    ++i;




    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax
    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax
    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax
    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax
    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax
    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax
    //FIXME write custom parser, maybe use a modifier version of the generic parser or add an "enum/struct" option to it to let it parse element syntax




    // Set the element length and return the node
    r->tokenBgn = index;
    r->tokenEnd = i - 1;
    return r;
}