#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"



namespace cmp {
    // std::atomic<bool> isCompilationProgressInitialized = false;
    SourceTree* compilePreprocessedSourceCode(pre::SegmentedCleanSource* b); //TODO fix return type and value
}