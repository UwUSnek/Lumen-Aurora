#include "treePhase.hpp"
#include "Parsers/namespace.hpp"
#include "Parsers/enum.hpp"
#include "Parsers/struct.hpp"
#include "Parsers/alias.hpp"




//TODO add error message function that takes 2 TokenCoord's (TokenizedSource and start+end indices)



#define dc dynamic_cast
void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {


    ulong i = 0;
    while((*b)[i].has_value()) {
        std::optional<Token> const &t = (*b)[i];


        // Known declarations
        if(dc<TK_Keyword*>(t->value)) {

            // Choose the parser function based on the keyword
            using parserFuncType = __base_ST* (*)(cmp::TokenizedSource*, ulong, ulong*);
            parserFuncType parserFunc;
            switch(t->getValue_Keyword()) {
                case ReservedTokenId::KEYWORD_NAMESPACE : { parserFunc = (parserFuncType)parseNamespace; break; }
                case ReservedTokenId::KEYWORD_ENUM      : { parserFunc = (parserFuncType)parseEnum;      break; }
                case ReservedTokenId::KEYWORD_STRUCT    : { parserFunc = (parserFuncType)parseStruct;    break; }
                case ReservedTokenId::KEYWORD_ALIAS     : { parserFunc = (parserFuncType)parseAlias;     break; }
                default : {
                    //FIXME error unexpected keyword
                }
            }

            // Call the parser function
            ++i; // Skip keyword //! Parsing starts from {
            ulong nodeLen;
            __base_ST* t = parserFunc(b, i, &nodeLen);
            r->lock(); r->cpp()->addChild(t); r->unlock();
            i += nodeLen;
        }


        // Identifiers (functions, operators, global variables)
        else if(dc<TK_Identifier*>(t->value)) {
            //! The identifier is not skipped as the parser needs to save it in the tree
            ulong nodeLen;
            __base_ST* t = nullptr; //FIXME actually parse it //FIXME pass nodeLen
            r->lock(); r->cpp()->addChild(t); r->unlock();
            i += nodeLen;
        }


        // Unexpected tokens
        else {
            //FIXME error unexpected token
        }
    }




    r->closePipe();
}
#undef dc