#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"



namespace pre {
    void __internal_startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r, StringPipe *rawCode);
    void            startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r, StringPipe *rawCode);

    void parseIncludeStatementName(ulong index, pre::SegmentedCleanSource *b, std::string &match);
    void parseIncludeStatementPath(ulong index, pre::SegmentedCleanSource *b, std::string &filePathMatch);
}