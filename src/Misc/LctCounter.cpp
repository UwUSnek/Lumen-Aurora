#include "LctCounter.hpp"








/**
 * @brief Calculates the length of the Line Continuation Token that strarts at index <index>.
 *     This function doesn't validate LCTs and doesn't print any error.
 *     Incomplete LCTs return 0.
 * @param b The .
 * @param index The index to check.
 * @return The number of characters the LCT occupies, or 0 if one was not found.
 */
ulong misc::measureLct(StringPipe &b, ulong index) {

    if(b[index] == '\\') {
        if(b.length() - 1 <= index) return 1;
        if(b[index + 1] == '\n') return 2;
    }
    return 0;
}