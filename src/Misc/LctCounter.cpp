#include "LctCounter.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"








/**
 * @brief Calculates the length of the Line Continuation Token that strarts at index <index>.
 *     This function doesn't validate LCTs and doesn't print any error.
 *     Incomplete LCTs return 0.
 * @param b The .
 * @param index The index to check.
 * @return The number of characters the LCT occupies, or 0 if one was not found.
 */
ulong misc::measureLct(pre::SegmentedCleanSource<false> &b, ulong index) {
    auto c0 = b[index];
    auto c1 = b[index + 1];

    if(c0 && *c0 == '\\') {
        if(!c1) return 1;
        if(*c1 == '\n') return 2;
    }
    return 0;
}