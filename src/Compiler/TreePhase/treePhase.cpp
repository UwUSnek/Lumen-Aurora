#include "treePhase.hpp"
#include "Utils/errors.hpp"
#include "Parsers/Generic/scope.hpp"



//FIXME add expressions





void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {
    //FIXME parse
    //FIXME print invalid places
    std::vector<__base_ST*> const &elms = generic_parseScope(b, 0, false, nullptr);
    // r->lock();
    // r->cpp()->addChild()
    // r->unlock();
    //TODO


    r->closePipe();
}
//TODO put const everywhere it's needed