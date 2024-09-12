#include "macroPhase.hpp"
#include "ALC.hpp"








void pre::startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r){
    b-> str.awaitClose(); //! Wait for include phase to finish to improve the progress estimation
    b->meta.awaitClose(); //! Wait for include phase to finish to improve the progress estimation


    ulong i = 0;
    while(b->str[i].has_value()) {
        r->str  += *b->str[i];
        r->meta += *b->meta[i];

        ++i;
        increaseLocalProgress(1);
    }


    r->str.closePipe();
    r->meta.closePipe();
}