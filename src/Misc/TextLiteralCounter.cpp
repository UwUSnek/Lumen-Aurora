#include "TextLiteralCounter.hpp"








/**
 * @brief Calculates the length of the string or char literal that strarts at index <index>.
 *     This function doesn't validate escapes or literals and doesn't print any error.
 *     Unterminated literals return the amount of characters from the opening sequence to the last available character.
 * @param b The buffer that contains the literal.
 * @param index The index at which the literal starts.
 * @return The length of the literal, including the opening and closing character sequences, or 0 if none was found.
 */
ulong misc::measureTextLiteral(StringPipe &b, ulong index) {
    if(b[index].has_value()) return 0;
    char literalType = b[index].value();
    if(literalType != '"' && literalType != '\'') return 0;


    ulong i = index + 1;
    while(true) {

        // Escape sequences
        //! No need for custom logic for \u escapes.
        //! Additional characters can be safely treated as normal text literal characters and skipped.
        if(b[i] == '\\') {
            i += 2;
        }

        // Closing sequence
        //! Macro definitions are skipped by the startCleanupPhase() function. No need to check.
        else if(b[i] == literalType) {
            ++i;
            break;
        }

        // Missing closing sequence
        //! No need to print error messages. Text literals are validated during the tokenization phase.
        else if(!b[i].has_value() || b[i] == '\n') {
            return std::min(i - 1, b.length() - 1) - index;
        }

        // Normal characters
        else {
            ++i;
        }
    }


    return i - index;
}

