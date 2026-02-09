#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void __internal_startLCTsPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
    void            startLCTsPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
}