#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startLineSplicingPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
    void            startLineSplicingPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
}