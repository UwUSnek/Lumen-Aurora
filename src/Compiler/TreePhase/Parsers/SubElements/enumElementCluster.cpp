#include "enumElementCluster.hpp"
#include "Utils/errors.hpp"








std::vector<ST_Sub_EnumElement*> cmp::parseEnumElementCluster(TokenizedSource* b, ulong index) {
    std::vector<ST_Sub_EnumElement*> r;

    // Save the starting identifier
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isIdentifier()) {
        return r;
    }
    ST_Sub_EnumElement* rElm = new ST_Sub_EnumElement();
    rElm->name = t0->getValue_Identifier();
    rElm->tokenBgn = index;
    rElm->tokenEnd = index; //FIXME count default value len

    //FIXME check default value
    r.push_back(t0);
    ulong i = index + 1;



//FIXME check if this function prints an error before other parsers can try
//FIXME check if other functions print errors before other parsers can try
    for(;; ++i) {
        std::optional<Token> const &t = (*b)[i];
        if(t.has_value()) {

            // Check ; keyword
            if(t->isKeyword(ReservedTokenId::KEYWORD_SEMICOLON)) {
                ++i;
                break;
            }

            // Check , keyword
            if(t->isKeyword(ReservedTokenId::KEYWORD_COMMA)) {
                ++i;

                std::optional<Token> const &t = (*b)[i];
                if(!t.has_value() || !t->isIdentifier()) {
                    utils::printError(
                        ERROR_CMP_ENUM_ELM_NO_NAME, utils::ErrType::COMPILER,
                        ElmCoords(b, index, i),
                        ElmCoords(b, i,     i),
                        "Incomplete definition of Enum element cluster.\n" + //TODO "enum element cluster" may be a little too technical for an error message. idk
                        (!t.has_value() ? "Expected an identifier, but the " + t->genDecoratedValue() + " was found instead." : "The identifier is missing.")
                    );
                }
                ST_Sub_EnumElement* rElm = new ST_Sub_EnumElement();
                rElm->name = t->getValue_Identifier();
                rElm->tokenBgn = i;
                rElm->tokenEnd = i; //FIXME count default value len
                r.push_back(t);
                ++i;

                //FIXME default value
                continue;
            }

            // No , or ;
            utils::printError(
                ERROR_CMP_ENUM_ELM_NO_TERMINATOR, utils::ErrType::COMPILER,
                ElmCoords(b, index, i),
                ElmCoords(b, i,     i),
                "Incomplete definition of Enum element cluster.\n" +
                (!t.has_value() ?
                    "Expected either a semicolon (;) or a comma (,), but the " + t->genDecoratedValue() + " was found instead." :
                    "Expected either a semicolon (;) or a comma (,)."
                )
            );
        }
    }




    // Return the list of nodes
    return r;
}