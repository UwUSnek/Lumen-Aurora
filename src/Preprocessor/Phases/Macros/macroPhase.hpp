#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void __internal_startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
    void            startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
}