#include "whitespaceCounter.hpp"
#include "Utils/Containers/StringPipe.hpp"








/**
 * @brief Calculates the number of valid consecutive whitespace characters in <b> starting from the index <index>. `[ \t\n]+`
 * @param b The string pipe that contains the whitespace characters.
 * @param index The starting index.
 * @return The amount of whitespace characters found. 0 if no whitespace was found.
 */
ulong misc::countWhitespace(StringPipe &b, ulong index) {
    ulong i = index;
    while(isWhitespaceChar(b[i])) ++i;
    return i - index;
}


bool misc::isWhitespaceChar(std::optional<char> const &c) {
    return c.has_value() && (c == ' ' || c == '\t' || c == '\n');
}


