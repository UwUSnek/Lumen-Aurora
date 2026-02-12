#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startLCTsPhase(ptr<SegmentedCleanSource<true>> b, ptr<SegmentedCleanSource<true>> r);
    void            startLCTsPhase(ptr<SegmentedCleanSource<true>> b, ptr<SegmentedCleanSource<true>> r);
}