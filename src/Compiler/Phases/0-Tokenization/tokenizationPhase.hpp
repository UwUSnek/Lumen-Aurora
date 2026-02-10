#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    void __internal_startTokenizationPhase(ptr<pre::SegmentedCleanSource> b, ptr<TokenizedSource> r);
    void            startTokenizationPhase(ptr<pre::SegmentedCleanSource> b, ptr<TokenizedSource> r);
}