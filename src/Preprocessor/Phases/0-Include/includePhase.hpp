#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"



namespace pre {
    void __internal_startIncludePhase(SegmentedCleanSource *b0, SegmentedCleanSource *r, ulong DBG_filePathIndex);
    void            startIncludePhase(SegmentedCleanSource *b,  SegmentedCleanSource *r, ulong DBG_filePathIndex);

    void parseIncludeStatementName(ulong index, pre::SegmentedCleanSource *b, std::string &match);
    void parseIncludeStatementPath(ulong index, pre::SegmentedCleanSource *b, std::string &filePathMatch);
}