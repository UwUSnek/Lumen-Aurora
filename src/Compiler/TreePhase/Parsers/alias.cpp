#include "alias.hpp"
#include "Generic/path.hpp"




cmp::ST_Alias* cmp::parseAlias(TokenizedSource* b, ulong index, ulong* elmLen) {
    ST_Alias* r = new ST_Alias();
    ulong i = index;


    // Parse original path
    ulong originalLen;
    ST_Path* original = parsePath(b, i, &originalLen);
    if(!originalLen) {
        //FIXME error expected a path but not found
    }
    else {
        r->original = original;
        i += originalLen;
    }


    // Parse "as" keyword
    std::optional<Token> const &t1 = (*b)[i];
    if(!t1.has_value() || !t1->isKeyword(ReservedTokenId::META_KEYWORD_AS)) {
        //FIXME error expected keyword "as" but not found
    }
    ++i;


    // Parse new name
    std::optional<Token> const &t2 = (*b)[i];
    if(!t2.has_value() || !t2->isIdentifier()) {
        //FIXME error expected an identifier but not found
    }
    else {
        r->name = t2->getValue_Identifier();
    }
    ++i;


    // Set the element length and return the node
    *elmLen = i - index;
    return r;
}