#pragma once
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/__base_Pattern.hpp"
#include "GenerationResult.hpp"
#include "Main/ALC.hpp"




namespace cmp {
    ptr<TreeGenerationResult> generateTree(__base_Pattern *pattern, ptr<TokenizedSource> b, ulong index, bool optional debug(, int indent));
    void __internal_startTreePhase(ptr<TokenizedSource> b, ptr<SourceTree> r);
    void            startTreePhase(ptr<TokenizedSource> b, ptr<SourceTree> r);
}