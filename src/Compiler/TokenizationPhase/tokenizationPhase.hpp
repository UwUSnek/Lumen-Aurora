#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"




namespace cmp {
    void __internal_startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r);
    void            startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r);
}