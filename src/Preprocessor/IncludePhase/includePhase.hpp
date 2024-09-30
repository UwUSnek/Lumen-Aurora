#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"



namespace pre {
    void startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
    void parseIncludeStatementName(ulong index, pre::SegmentedCleanSource *b, std::string &match);
    void parseIncludeStatementPath(ulong index, pre::SegmentedCleanSource *b, std::string &filePathMatch);
}