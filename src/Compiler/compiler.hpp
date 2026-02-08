#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/Tree/SourceTree.hpp"



namespace cmp {
    // std::atomic<bool> isCompilationProgressInitialized = false;
    SourceTree* compilePreprocessedSourceCode(pre::SegmentedCleanSource* b); //TODO fix return type and value
}