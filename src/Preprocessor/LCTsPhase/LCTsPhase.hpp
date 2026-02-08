#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void __internal_startLCTsPhase(StringPipe *b, ulong DBG_filePathIndex, SegmentedCleanSource *r);
    void            startLCTsPhase(StringPipe *b, ulong DBG_filePathIndex, SegmentedCleanSource *r);
}