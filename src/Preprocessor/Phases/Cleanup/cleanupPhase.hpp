#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void __internal_startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
    void            startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);

    ulong measureComment(StringPipe &b, ulong index);
    ulong saveLiteral(SegmentedCleanSource *b, ulong index, SegmentedCleanSource *r);
}