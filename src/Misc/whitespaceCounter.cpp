#include "whitespaceCounter.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"








/**
 * @brief Calculates the number of valid consecutive whitespace characters in <b> starting from the index <index>. `[ \t\n]+`
 * @param b The string pipe that contains the whitespace characters.
 * @param index The starting index.
 * @return The amount of whitespace characters found. 0 if no whitespace was found.
 */
ulong misc::countWhitespace(pre::SegmentedCleanSource<false> &b, ulong index) {
    ulong i = index;
    while(b[i] && isWhitespaceChar(b[i]->c)) {
        ++i;
    }
    return i - index;
}


bool misc::isWhitespaceChar(char const &c) {
    return c == ' ' || c == '\t' || c == '\n';
}


