#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);

    ulong measureComment(StringPipe &b, ulong index);
    ulong saveLiteral(SegmentedCleanSource *b, ulong index, SegmentedCleanSource *r);
}