#include "whitespaceCounter.hpp"








/**
 * @brief Counts the number of valid consecutive whitespace characters in <b> starting from the index <index>
 *      [ \t\n]+
 * @param b The source code buffer.
 * @param index The starting index.
 * @return The amount of whitespace characters found. 0 if no whitespace was found.
 */
ulong cmp::countWhitespace(pre::SegmentedCleanSource *b, ulong index) {
    ulong i = index;
    while(isWhitespaceChar(b->str[i])) ++i;
    return i - index;
}


bool cmp::isWhitespaceChar(std::optional<char> const &c) {
    return c.has_value() && (c == ' ' || c == '\t' || c == '\n');
}


