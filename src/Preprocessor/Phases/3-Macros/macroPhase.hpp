#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startMacroPhase(ptr<SegmentedCleanSource<true>> b, ptr<SegmentedCleanSource<true>> r);
    void            startMacroPhase(ptr<SegmentedCleanSource<true>> b, ptr<SegmentedCleanSource<true>> r);
}