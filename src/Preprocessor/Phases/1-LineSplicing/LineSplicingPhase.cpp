#include "Preprocessor/Phases/1-LineSplicing/LineSplicingPhase.hpp"
#include "Main/ALC.hpp"
#include "Main/FatalErrorException.hpp"
#include "Misc/LstCounter.hpp"
#include "Misc/whitespaceCounter.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"














void pre::__internal_startLineSplicingPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    ulong i = 0;
    while((*b)[i]) {
        ulong skipLen = 0;


        // Custom logic for LJTs (they need backwards lookup)
        //! Backwards lookup not actually implemented bc of design constraints.
        //! Instead, this checks for whitespace sequences and stores them if not removed by an adjacent LJT.
        //! Removes both the whitespace and LJT otherwise.
        const ulong whitespaceL = misc::countWhitespace(*b, i);
        if(const ulong ljt = misc::measureLjt(*b, i + whitespaceL); ljt) {
            const ulong whitespaceR = misc::countWhitespace(*b, i + whitespaceL + ljt);
            skipLen = whitespaceL + ljt + whitespaceR;
            //! Set skipLen and let the if(skipLen) barch handle it
        }
        else if(whitespaceL) {
            for(ulong j = 0; j < whitespaceL; ++j) {
                *r += *(*b)[i + j];
            }
            increaseLocalProgress(whitespaceL);
            i += whitespaceL;
            continue;
            //! Set data manually and go to the next iteration
        }


        // Skip LCT / LJT
        if(!skipLen) skipLen = misc::measureLct(*b, i);
        if(skipLen) {
            using enum PhaseID;
            decreaseMaxProgress(skipLen, P2_Cleanup, P3_Macros, C0_Tokenization);
            increaseLocalProgress(skipLen);
            i += skipLen;
        }


        // Push normal characters
        else {
            increaseLocalProgress(1);
            *r += *(*b)[i];
            ++i;
        }


        //! Macro definitions and invocations cannot paste LSTs.
        //! LSTs are removed before macros are parsed.
        //! No need to check them.
    }
}







void pre::startLineSplicingPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    // Try to execute the subphase
    try {
        __internal_startLineSplicingPhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipes and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        flagLocalError();
    }
}