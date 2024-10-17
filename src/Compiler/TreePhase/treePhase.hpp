#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Compiler/TreePhase/__base_Pattern.hpp"
#include "GenerationResult.hpp"




namespace cmp {
    GenerationResult *generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool optional debug(, int indent));
    void startTreePhase(TokenizedSource *b, SourceTree *r);
}