#include "ALC.hpp"
#include "LCTsPhase.hpp"
#include "Utils/ansi.hpp"








/**
 * @brief Checks if a Line continuation token is present in the string <b> at the index <index> and calculates its length.
 * @param b The stirng.
 * @param index The index to check.
 * @return The number of characters the LCT occupies, or 0 if one was not found.
 */
ulong checkLct(StringPipe *b, ulong index) {

    if((*b)[index] == '\\') {
        if(b->length() - 1 <= index) return 1;
        if((*b)[index + 1] == '\n') return 2;
    }
    return 0;
}








void pre::__internal_startLCTsPhase(StringPipe *b, ulong DBG_filePathIndex, SegmentedCleanSource *r) {
    // increaseMaxProgress(b->length());


    ulong i = 0;        // The current index relative to the raw data
    ulong c = 0;        // The current column number relative to the raw data
    ulong l = 0;        // The current line number relative to the raw data
    // while(i < b->length()) {
    while((*b)[i].has_value()) {

        // Skip LCTs
        ulong lct = checkLct(b, i);
        if(lct) {
            decreaseMaxProgress(Preprocessor_Cleanup, lct);
            decreaseMaxProgress(Preprocessor_Macros, lct);
            decreaseMaxProgress(Compiler_Tokenization, lct);
            increaseLocalProgress(lct);
            i += lct;
            c += lct;
            ++l;
        }

        // Push normal characters
        else {
            increaseLocalProgress(1);
            r->str  += (*b)[i].value();
            r->meta += CleanSourceMeta(i, l, c, DBG_filePathIndex);
            if((*b)[i] == '\n') {
                ++l;
                c = 0;
            }
            ++i;
            ++c;
        }
    }
}







void pre::startLCTsPhase(StringPipe *b, ulong DBG_filePathIndex, SegmentedCleanSource *r) {

    // Try to execute the subphase
    try {
        __internal_startLCTsPhase(b, DBG_filePathIndex, r);
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