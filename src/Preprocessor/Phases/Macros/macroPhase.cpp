#include "macroPhase.hpp"
#include "ALC.hpp"
#include "Utils/ansi.hpp"








void pre::__internal_startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r){
    ulong i = 0;
    while(b->str[i].has_value()) {
        increaseLocalProgress(1);
        r->str  += *b->str[i];
        r->meta += *b->meta[i];
        ++i;
        // decreaseMaxProgress(Compilation, definition length); //TODO
        // decreaseMaxProgress(Compilation, call length); //TODO
    }
}








void pre::startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r){

    // Try to execute the subphase
    try {
        __internal_startMacroPhase(b, r);
        r->str.closePipe();
        r->meta.closePipe();
    }

    // If errors occur, close the return pipe sand return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->str.closePipe();
        r->meta.closePipe();
        std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Preprocessing_A].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}