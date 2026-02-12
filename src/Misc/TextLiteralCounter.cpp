#include "TextLiteralCounter.hpp"








/**
 * @brief Calculates the length of the string or char literal that strarts at index <index>.
 *     This function doesn't validate escapes or literals and doesn't print any error.
 *     Unterminated literals return the amount of characters from the opening sequence to the last available character.
 * @param b The buffer that contains the literal.
 * @param index The index at which the literal starts.
 * @return The length of the literal, including the opening and closing character sequences, or 0 if none was found.
 */
ulong misc::measureTextLiteral(pre::SegmentedCleanSource<false> &b, ulong index) {
    if(b[index]) return 0;
    char literalType = b[index]->c;
    if(literalType != '"' && literalType != '\'') return 0;


    ulong i = index + 1;
    while(true) {

        // Missing closing sequence
        //! No need to print error messages. Text literals are validated during the tokenization phase.
        if(!b[i] || b[i]->c == '\n') {
            return std::min(i - 1, b.length() - 1) - index;
        }
        const char c = b[i]->c;


        // Escape sequences
        //! No need for custom logic for \u escapes.
        //! Additional characters can be safely treated as normal text literal characters and skipped.
        if(c == '\\') {
            i += 2;
        }

        // Closing sequence
        //! Macro definitions are skipped by the startCleanupPhase() function. No need to check.
        else if(c == literalType) {
            ++i;
            break;
        }

        // Normal characters
        else {
            ++i;
        }
    }


    return i - index;
}

