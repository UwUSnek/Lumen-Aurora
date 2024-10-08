#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"




namespace cmp {
    void startTreePhase(TokenizedSource *b, SourceTree *r);
}