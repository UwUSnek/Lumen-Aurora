#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/1-Tree/SourceTree.hpp"



namespace cmp {
    ptr<SourceTree> compilePreprocessedSourceCode(ptr<pre::SegmentedCleanSource<false>> b);
}