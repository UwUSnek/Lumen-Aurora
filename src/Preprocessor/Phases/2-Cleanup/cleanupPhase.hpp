#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startCleanupPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
    void            startCleanupPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
}