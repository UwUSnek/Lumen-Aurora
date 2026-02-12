#include "Preprocessor/Phases/1-LCTs/LCTsPhase.hpp"
#include "Main/ALC.hpp"
#include "Main/FatalErrorException.hpp"
#include "Misc/LctCounter.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"














void pre::__internal_startLCTsPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    ulong i = 0;
    while((*b)[i]) {

        // Skip LCTs
        ulong lct = misc::measureLct(*b, i);
        if(lct) {
            using enum PhaseID;
            decreaseMaxProgress(Preprocessor_Cleanup,  lct);
            decreaseMaxProgress(Preprocessor_Macros,   lct);
            decreaseMaxProgress(Compiler_Tokenization, lct);
            increaseLocalProgress(lct);
            i += lct;
        }

        // Push normal characters
        else {
            increaseLocalProgress(1);
            *r += *(*b)[i];
            ++i;
        }

        //! Macro definitions and invocations cannot paste LCTs.
        //! LCTs are removed before macros are parsed.
        //! No need to check them.
    }
}







void pre::startLCTsPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    // Try to execute the subphase
    try {
        __internal_startLCTsPhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipes and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        // std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Preprocessing_A].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}