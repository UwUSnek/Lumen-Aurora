#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"




namespace cmp {
    ST_Namespace* parseNamespace(TokenizedSource* b, ulong index);
}