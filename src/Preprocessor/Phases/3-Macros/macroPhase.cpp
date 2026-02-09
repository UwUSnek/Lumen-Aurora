#include "macroPhase.hpp"
#include "Main/ALC.hpp"
#include "Main/FatalErrorException.hpp"
#include "Utils/ansi.hpp"








void pre::__internal_startMacroPhase(SegmentedCleanSource *b, SegmentedCleanSource *r){
    ulong i = 0;
    while(b->str[i].has_value()) {

        // FIXME
        // Parse and remove macro definitions, store them locally
        // #define name...\n     // Can include anything, including " and '
        // #name(...)            // Can include valid indentifiers, valid tokens, and `-limited parameters (which can contain anything)


        // FIXME
        // Replace macro invocations with the fully expanded code


        increaseLocalProgress(1);
        r->str  += *b->str[i];
        r->meta += *b->meta[i];
        ++i;
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