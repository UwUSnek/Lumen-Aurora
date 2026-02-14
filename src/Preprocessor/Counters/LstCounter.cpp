#include "LstCounter.hpp"
#include "Preprocessor/AnnotatedSource.hpp"







/**
 * @brief Calculates the length of the Line Junction Token that starts at index <index> of the buffer <b>.
 *     This doesn't include adjacent whitespace characters.
 *     This function doesn't validate LJTs and doesn't print any error.
 *     Incomplete LJTs return 0.
 * @param b The buffer to read characters from.
 * @param index The index to start reading from.
 * @return The number of characters the LJT occupies, or 0 if one was not found.
 */
ulong misc::measureLjt(pre::AnnotatedSource<false> &b, ulong index) {
    auto c0 = b[index];
    auto c1 = b[index + 1];
    auto c2 = b[index + 2];

    if(c0 && *c0 == '\\') {
        if(!c1) return 0;
        if(c1 == '\\') {
            if(!c2) return 2;
            if(*c2 == '\n') return 3;
        }
    }
    return 0;
}




/**
 * @brief Calculates the length of the Line Continuation Token that starts at index <index> of the buffer <b>.
 *     This function doesn't validate LCTs and doesn't print any error.
 *     Incomplete LCTs return 0.
 * @param b The buffer to read characters from.
 * @param index The index to start reading from.
 * @return The number of characters the LCT occupies, or 0 if one was not found.
 */
ulong misc::measureLct(pre::AnnotatedSource<false> &b, ulong index) {
    auto c0 = b[index];
    auto c1 = b[index + 1];

    if(c0 && *c0 == '\\') {
        if(!c1) return 1;
        if(*c1 == '\n') return 2;
    }
    return 0;
}