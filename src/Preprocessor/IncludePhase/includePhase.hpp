#pragma once
#include <string>
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"



namespace pre {
    void startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
    void parseIncludeStatementName(ulong i, pre::SegmentedCleanSource *b, std::string &match);
    void parseIncludeStatementPath(ulong i, pre::SegmentedCleanSource *b, std::string &filePathMatch);
}