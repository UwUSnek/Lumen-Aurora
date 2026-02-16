#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void __internal_startMacroPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
    void            startMacroPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r);
}