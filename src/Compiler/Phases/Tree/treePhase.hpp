#pragma once
#include "Compiler/Phases/Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/Tree/SourceTree.hpp"
#include "Compiler/Phases/Tree/Patterns/__base_Pattern.hpp"
#include "GenerationResult.hpp"
#include "ALC.hpp"




namespace cmp {
    TreeGenerationResult *generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool optional debug(, int indent));
    void __internal_startTreePhase(TokenizedSource *b, SourceTree *r);
    void            startTreePhase(TokenizedSource *b, SourceTree *r);
}