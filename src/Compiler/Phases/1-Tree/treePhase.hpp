#pragma once
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/__base_Pattern.hpp"
#include "GenerationResult.hpp"
#include "Main/ALC.hpp"




namespace cmp {
    ptr<TreeGenerationResult> generateTree(__base_Pattern *pattern, ptr<TokenizedSource<false>> b, ulong index, bool optional debug(, ulong indent));
    void __internal_startTreePhase(ptr<TokenizedSource<false>> b, ptr<SourceTree> r);
    void            startTreePhase(ptr<TokenizedSource<false>> b, ptr<SourceTree> r);
}