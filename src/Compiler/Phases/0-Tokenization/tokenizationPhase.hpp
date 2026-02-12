#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    void __internal_startTokenizationPhase(ptr<pre::SegmentedCleanSource<true>> b, ptr<TokenizedSource<true>> r);
    void            startTokenizationPhase(ptr<pre::SegmentedCleanSource<true>> b, ptr<TokenizedSource<true>> r);
}