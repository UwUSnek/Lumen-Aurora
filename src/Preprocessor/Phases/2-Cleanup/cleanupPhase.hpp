#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startCleanupPhase(ptr<SegmentedCleanSource<true>> b, ptr<SegmentedCleanSource<true>> r);
    void            startCleanupPhase(ptr<SegmentedCleanSource<true>> b, ptr<SegmentedCleanSource<true>> r);
}