#include "macroPhase.hpp"
#include "ALC.hpp"








void pre::startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r){
    ulong i = 0;
    while(b->str[i].has_value()) {
        r->str  += *b->str[i];
        r->meta += *b->meta[i];

        ++i;
        increaseLocalProgress(1);
        // decreaseMaxProgress(Compilation, definition length); //TODO
        // decreaseMaxProgress(Compilation, call length); //TODO
    }


    r->str.closePipe();
    r->meta.closePipe();
}