#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"




namespace cmp {
    ST_Alias* parseAlias(TokenizedSource* b, ulong index);
}