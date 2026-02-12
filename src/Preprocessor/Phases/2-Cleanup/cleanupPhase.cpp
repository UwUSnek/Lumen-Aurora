#include "Main/ALC.hpp"

#include "cleanupPhase.hpp"
#include "Main/FatalErrorException.hpp"
#include "Misc/TextLiteralCounter.hpp"
#include "Misc/CommentCounter.hpp"








void pre::__internal_startCleanupPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    ulong i = 0;
    while((*b)[i]) {


        // Skip (and remove) comments
        if(ulong commentLen = misc::measureComment(*b, i); commentLen) {
            decreaseMaxProgress(PhaseID::Preprocessor_Macros,   commentLen);
            decreaseMaxProgress(PhaseID::Compiler_Tokenization, commentLen);
            increaseLocalProgress(commentLen);
            i += commentLen;
            continue;
        }


        // Skip (and preserve) literals
        if(auto literalLen = misc::measureTextLiteral(*b, i); literalLen) {
            increaseLocalProgress(literalLen);
            for(ulong j = 0; j < literalLen; ++j) {
                *r += *(*b)[i + j];
            }
            i += literalLen;
            continue;
        }


        // FIXME
        // Skip and store macro definitions and invocations
        // #define name...\n     // Can include anything, including " and '
        // #name(...)            // Can include valid indentifiers, valid tokens, and `-limited parameters (which can contain anything)


        // Save normal characters
        increaseLocalProgress(1);
        *r += *(*b)[i];
        ++i;
    }
}








void pre::startCleanupPhase(ptr<AnnotatedSource<false>> b, ptr<AnnotatedSource<false>> r) {

    // Try to execute the subphase
    try {
        __internal_startCleanupPhase(b, r);
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