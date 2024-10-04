#include "treePhase.hpp"
#define dc dynamic_cast








void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {


    ulong i = 0;
    while((*b)[i].has_value()) {
        std::optional<Token> const &t = (*b)[i];

        if(dc<TK_Keyword*>(t->value)) {}
    }




    r->closePipe();
}



#undef dc