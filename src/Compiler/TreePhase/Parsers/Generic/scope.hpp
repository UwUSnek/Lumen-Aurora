#pragma once
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"




namespace cmp {
    std::vector<__base_ST*> parseScope(TokenizedSource *b, ulong index, bool checkCurly, ulong* scopeLen);
}