#include "ALC.hpp"
#include "LCTsPhase.hpp"
#include "Preprocessor/preprocessor.hpp"








namespace pre {
    /**
     * @brief Checks if a Line continuation token is present in the string <b> at the index <index> and calculates its length.
     * @param b The stirng.
     * @param index The index to check.
     * @return The number of characters the LCT occupies, or 0 if one was not found.
     */
    ulong checkLct(std::string const *b, ulong index) {
        if((*b)[index] == '\\' && (*b)[index + 1] == '\n') return 2;
        if((*b)[index] == '\0') return 1;
        return 0;
    }








    void startLCTsPhase(std::string const *b, ulong DBG_filePathIndex, SegmentedCleanSource *r) {
        increaseMaxProgress(b->length());


        ulong i = 0;                // Current index relative to the raw data
        ulong curLine = 0;          // The current line number relative to the raw data
        while(i < b->length()) {

            // Skip LCTs
            ulong lct = checkLct(b, i);
            if(lct) {
                i += lct;
                increaseLocalProgress(lct);
                ++curLine;
            }

            // Push normal characters
            else {
                r->str  += (*b)[i];
                r->meta += CleanSourceMeta(CleanSourceType::MISC, i, curLine, DBG_filePathIndex);
                if(b->at(i) == '\n') ++curLine;
                ++i;
                // pre::increaseLocalProgress(1);
            }
        }


        // return r;
        r->str.closePipe();
        r->meta.closePipe();
    }
}