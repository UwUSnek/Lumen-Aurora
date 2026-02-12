#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startMacroPhase(ptr<SegmentedCleanSource<false>> b, ptr<SegmentedCleanSource<false>> r);
    void            startMacroPhase(ptr<SegmentedCleanSource<false>> b, ptr<SegmentedCleanSource<false>> r);
}