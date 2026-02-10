#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startMacroPhase(ptr<SegmentedCleanSource> b, ptr<SegmentedCleanSource> r);
    void            startMacroPhase(ptr<SegmentedCleanSource> b, ptr<SegmentedCleanSource> r);
}