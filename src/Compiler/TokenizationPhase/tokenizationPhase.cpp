#include "ALC.hpp"
#include "tokenizationPhase.hpp"



void cmp::startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r) {
    // increaseMaxProgress()
    ulong i = 0;
    while(b->str[i].has_value()) {
        increaseLocalProgress(1);
        cout << "test";
        ++i;
    }
}