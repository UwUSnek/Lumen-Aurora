#include "treePhase.hpp"
#define dc dynamic_cast

#include "Parsers/namespace.hpp"
#include "Parsers/enum.hpp"
#include "Parsers/struct.hpp"
#include "Parsers/alias.hpp"




//TODO add error message function that takes 2 TokenCoord's (TokenizedSource and start+end indices)




void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {


    ulong i = 0;
    while((*b)[i].has_value()) {
        std::optional<Token> const &t = (*b)[i];

        // Known declarations
        if(dc<TK_Keyword*>(t->value)) {
            switch(t->getValue_Keyword()) {
                case ReservedTokenId::KEYWORD_NAMESPACE : {
                    __base_ST* t = parseNamespace(b, i);
                    r->lock(); r->cpp()->addChild(t); r->unlock();
                    break;
                }
                case ReservedTokenId::KEYWORD_ENUM : {
                    __base_ST* t = parseEnum(b, i);
                    r->lock(); r->cpp()->addChild(t); r->unlock();
                    break;
                }
                case ReservedTokenId::KEYWORD_STRUCT : {
                    __base_ST* t = parseStruct(b, i);
                    r->lock(); r->cpp()->addChild(t); r->unlock();
                    break;
                }
                case ReservedTokenId::KEYWORD_ALIAS : {
                    __base_ST* t = parseAlias(b, i);
                    r->lock(); r->cpp()->addChild(t); r->unlock();
                    break;
                }
                default : {
                    //FIXME error unexpected keyword
                }
            }
        }
        else if(dc<TK_Identifier*>(t->value)) {
            __base_ST* t = nullptr; //FIXME actually parse it
            r->lock(); r->cpp()->addChild(t); r->unlock();
        }
        else {
            //FIXME error unexpected token
        }
    }




    r->closePipe();
}


#undef dc