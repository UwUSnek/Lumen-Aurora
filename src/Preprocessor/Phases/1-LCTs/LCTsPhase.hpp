#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startLCTsPhase(ptr<SegmentedCleanSource<false>> b, ptr<SegmentedCleanSource<false>> r);
    void            startLCTsPhase(ptr<SegmentedCleanSource<false>> b, ptr<SegmentedCleanSource<false>> r);
}