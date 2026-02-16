#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"



namespace pre {
    void __internal_startIncludePhase(ptr<AnnotatedSource<false>> b0, ptr<AnnotatedSource<false>> r);
    void            startIncludePhase(ptr<AnnotatedSource<false>> b,  ptr<AnnotatedSource<false>> r);
}