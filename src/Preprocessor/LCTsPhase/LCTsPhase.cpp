#include "ALC.hpp"
#include "LCTsPhase.hpp"








/**
 * @brief Checks if a Line continuation token is present in the string <b> at the index <index> and calculates its length.
 * @param b The stirng.
 * @param index The index to check.
 * @return The number of characters the LCT occupies, or 0 if one was not found.
 */
ulong checkLct(std::string const *b, ulong index) {

    if((*b)[index] == '\\') {
        if(b->length() - 1 <= index) return 1;
        if((*b)[index + 1] == '\n') return 2;
    }
    return 0;
}








void pre::startLCTsPhase(std::string const *b, ulong DBG_filePathIndex, SegmentedCleanSource *r) {
    increaseMaxProgress(b->length());


    ulong i = 0;        // The current index relative to the raw data
    ulong c = 0;        // The current column number relative to the raw data
    ulong l = 0;        // The current line number relative to the raw data
    while(i < b->length()) {

        // Skip LCTs
        ulong lct = checkLct(b, i);
        if(lct) {
            i += lct;
            c += lct;
            increaseLocalProgress(lct);
            ++l;
        }

        // Push normal characters
        else {
            r->str  += (*b)[i];
            r->meta += CleanSourceMeta(i, l, c, DBG_filePathIndex);
            if(b->at(i) == '\n') {
                ++l;
                c = 0;
            }
            ++i;
            ++c;
        }
    }


    // return r;
    r->str.closePipe();
    r->meta.closePipe();
}