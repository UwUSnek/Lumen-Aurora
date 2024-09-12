#pragma once
#include <string>
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"




namespace pre {
    void startLCTsPhase(std::string const *b, ulong DBG_filePathIndex, SegmentedCleanSource *r);
}