#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"




namespace cmp {
    void startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r);
}