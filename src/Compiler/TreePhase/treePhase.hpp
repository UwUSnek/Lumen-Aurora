#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"




namespace cmp {
    __base_ST* generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool fatal debug(, int indent));
    void startTreePhase(TokenizedSource *b, SourceTree *r);
}