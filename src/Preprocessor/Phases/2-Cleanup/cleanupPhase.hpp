#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startCleanupPhase(ptr<SegmentedCleanSource> b, ptr<SegmentedCleanSource> r);
    void            startCleanupPhase(ptr<SegmentedCleanSource> b, ptr<SegmentedCleanSource> r);
}