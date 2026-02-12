#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"



namespace pre {
    void __internal_startIncludePhase(ptr<SegmentedCleanSource<true>> b0, ptr<SegmentedCleanSource<true>> r);
    void            startIncludePhase(ptr<SegmentedCleanSource<true>> b,  ptr<SegmentedCleanSource<true>> r);
}