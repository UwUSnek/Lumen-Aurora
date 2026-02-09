#include "ALC.hpp"

#include "cleanupPhase.hpp"
#include "FatalErrorException.hpp"
#include "Misc/TextLiteralCounter.hpp"
#include "Misc/CommentCounter.hpp"








void pre::__internal_startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

    ulong i = 0;
    while(b->str[i].has_value()) {


        // Skip (and remove) comments
        if(ulong commentLen = misc::measureComment(b->str, i); commentLen) {
            decreaseMaxProgress(Preprocessor_Macros, commentLen);
            decreaseMaxProgress(Compiler_Tokenization, commentLen);
            increaseLocalProgress(commentLen);
            i += commentLen;
            continue;
        }


        // Skip (and preserve) literals
        if(auto literalLen = misc::measureTextLiteral(b->str, i); literalLen) {
            increaseLocalProgress(literalLen);
            for(int j = 0; j < literalLen; ++j) {
                r->str  += b->str [i + j].value();
                r->meta += b->meta[i + j].value();
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
        r->str  += *b->str[i];
        r->meta += *b->meta[i];
        ++i;
    }
}








void pre::startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

    // Try to execute the subphase
    try {
        __internal_startCleanupPhase(b, r);
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