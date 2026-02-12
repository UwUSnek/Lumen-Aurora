#include "macroPhase.hpp"
#include "Main/ALC.hpp"
#include "Main/FatalErrorException.hpp"








void pre::__internal_startMacroPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r){
    ulong i = 0;
    while((*b)[i]) {

        // FIXME
        // Parse and remove macro definitions, store them locally
        // #define name...\n     // Can include anything, including " and '
        // #name(...)            // Can include valid indentifiers, valid tokens, and `-limited parameters (which can contain anything)


        // FIXME
        // Replace macro invocations with the fully expanded code


        increaseLocalProgress(1);
        *r += *(*b)[i];
        ++i;
    }
}








void pre::startMacroPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r){

    // Try to execute the subphase
    try {
        __internal_startMacroPhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipe sand return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        // std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Preprocessing_A].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}