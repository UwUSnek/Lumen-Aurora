#include "path.hpp"
//FIXME free the tokens after using them
//FIXME also free the other stuff




cmp::ST_Sub_Path* cmp::parsePath(TokenizedSource* b, ulong index, ulong* elmLen) {
    ST_Sub_Path *r = new ST_Sub_Path();




    // Save the starting identifier
    std::optional<Token> const &t0 = (*b)[index];
    if(!t0.has_value() || !t0->isIdentifier()) {
        elmLen = 0;
        return nullptr;
    }
    else {
        r->idList.push_back(t0->getValue_Identifier());
    }




    // Save subsequent separator-identifier pairs
    ulong i = index;
    while(true) {


        // Skip . separator, return if one is not found
        std::optional<Token> const &ta = (*b)[i];
        if(!ta.has_value() || !ta->isKeyword(ReservedTokenId::KEYWORD_DOT)) break;
        ++i;


        // Save identifier, print an error if one is not found
        std::optional<Token> const &tb = (*b)[i];
        if(!tb.has_value()) {
            //FIXME error unexpected end of module
        }
        else if(!tb->isIdentifier()) {
            //FIXME error unexpected token
        }
        else {
            r->idList.push_back(tb->getValue_Identifier());
        }
        ++i;
    }




    // Set element length and return the node
    *elmLen = i - index;
    return r;
}