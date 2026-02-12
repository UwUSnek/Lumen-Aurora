#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"



namespace pre {
    void __internal_startIncludePhase(ptr<SegmentedCleanSource<false>> b0, ptr<SegmentedCleanSource<false>> r);
    void            startIncludePhase(ptr<SegmentedCleanSource<false>> b,  ptr<SegmentedCleanSource<false>> r);
}