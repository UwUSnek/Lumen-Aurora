#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    void __internal_startTokenizationPhase(ptr<pre::AnnotatedSource<false>> b, ptr<TokenizedSource<false>> r);
    void            startTokenizationPhase(ptr<pre::AnnotatedSource<false>> b, ptr<TokenizedSource<false>> r);
}