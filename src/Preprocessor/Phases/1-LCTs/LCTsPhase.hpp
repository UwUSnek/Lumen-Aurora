#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startLCTsPhase(ptr<SegmentedCleanSource> b, ptr<SegmentedCleanSource> r);
    void            startLCTsPhase(ptr<SegmentedCleanSource> b, ptr<SegmentedCleanSource> r);
}