#include "treePhase.hpp"




void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {
    b->awaitClose(); //TODO remove after writing this and the other subphases
    r->closePipe();
}