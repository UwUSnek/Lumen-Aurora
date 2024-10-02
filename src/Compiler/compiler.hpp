#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"



namespace cmp {
    // std::atomic<bool> isCompilationProgressInitialized = false;
    void compilePreprocessedSourceCode(pre::SegmentedCleanSource* b);
}