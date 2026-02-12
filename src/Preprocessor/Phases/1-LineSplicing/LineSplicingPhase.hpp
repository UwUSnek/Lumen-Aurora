#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startLineSplicingPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
    void            startLineSplicingPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
}