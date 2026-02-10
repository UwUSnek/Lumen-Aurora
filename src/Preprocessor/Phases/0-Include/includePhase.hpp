#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"



namespace pre {
    void __internal_startIncludePhase(ptr<SegmentedCleanSource> b0, ptr<SegmentedCleanSource> r);
    void            startIncludePhase(ptr<SegmentedCleanSource> b,  ptr<SegmentedCleanSource> r);

    void parseIncludeStatementName(ulong index, ptr<SegmentedCleanSource> b, std::string &match);
    void parseIncludeStatementPath(ulong index, ptr<SegmentedCleanSource> b, std::string &filePathMatch);
}