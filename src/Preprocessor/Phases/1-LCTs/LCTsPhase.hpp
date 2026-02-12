#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startLCTsPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
    void            startLCTsPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
}