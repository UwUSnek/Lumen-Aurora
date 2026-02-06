#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void __internal_startLCTsPhase(const std::string *b, ulong DBG_filePathIndex, SegmentedCleanSource *r);
    void            startLCTsPhase(const std::string *b, ulong DBG_filePathIndex, SegmentedCleanSource *r);
}