#pragma once
#include <string>
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"




namespace pre {
    SegmentedCleanSource startLCTsPhase(std::string b, ulong DBG_filePathIndex);
}