#include "ALC.hpp"
#include "FatalErrorException.hpp"
#include "Misc/LctCounter.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "LCTsPhase.hpp"














void pre::__internal_startLCTsPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {
    // increaseMaxProgress(b->length());


    // while(i < b->length()) {
    ulong i = 0;
    while(b->str[i].has_value()) {

        // Skip LCTs
        ulong lct = misc::measureLct(b->str, i);
        if(lct) {
            decreaseMaxProgress(Preprocessor_Cleanup, lct);
            decreaseMaxProgress(Preprocessor_Macros, lct);
            decreaseMaxProgress(Compiler_Tokenization, lct);
            increaseLocalProgress(lct);
            i += lct;
        }

        // Push normal characters
        else {
            increaseLocalProgress(1);
            r->str  += b->str[i].value();
            r->meta += b->meta[i].value();
        }

        //! Macro definitions and invocations cannot paste LCTs.
        //! LCTs are removed before macros are parsed.
        //! No need to check them.
    }
}







void pre::startLCTsPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

    // Try to execute the subphase
    try {
        __internal_startLCTsPhase(b, r);
        r->str.closePipe();
        r->meta.closePipe();
    }

    // If errors occur, close the return pipes and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->str.closePipe();
        r->meta.closePipe();
        std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Preprocessing_A].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}