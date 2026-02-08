#include "ALC.hpp"

#include "cleanupPhase.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"








void pre::__internal_startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

    ulong i = 0;
    while(b->str[i].has_value()) {

        // Skip comments
        if(ulong commentLen = measureComment(b->str, i); commentLen) {
            decreaseMaxProgress(Preprocessor_Macros, commentLen);
            decreaseMaxProgress(Compiler_Tokenization, commentLen);
            increaseLocalProgress(commentLen);
            i += commentLen;
            continue;
        }

        // Skip (and preserve) literals
        if(ulong literalLen = saveLiteral(b, i, r); literalLen) {
            decreaseMaxProgress(Preprocessor_Macros, literalLen);
            decreaseMaxProgress(Compiler_Tokenization, literalLen);
            increaseLocalProgress(literalLen);
            i += literalLen;
            continue;
        }

        // Skip (and preserve) macro definitions and calls
        // FIXME

        // Save normal characters
        increaseLocalProgress(1);
        r->str  += *b->str[i];
        r->meta += *b->meta[i];
        ++i;
    }
}








/**
 * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
 * @param b The string pipe that contains the comment.
 * @param index The index at which the comment starts.
 * @return The length of the comment, including the length of the opening and closing character sequences (not \0 or \n).
 *     If the buffer doesn't contain a comment that starts at index <index>, 0 is returned.
 */
ulong pre::measureComment(StringPipe &b, ulong index) {
    if(b[index] != '/') return 0;


    char last = *b[index];
    char commType = '\0'; // '\0' if unknow, '/' if single line, '*' if multiline
    ulong i = index + 1;
    while(true) {

        // Starting sequence
        if(commType == '\0') {
            if(b[i] == '/' || b[i] == '*') {
                commType = *b[i];
                continue;
            }
            else {  //! Starting sequence not found (this includes \n and \0 cases)
                return 0;
            }
        }

        // Single character closing sequences (End of file or single line comments)
        else if(!b[i].has_value() || commType == '/' && b[i] == '\n') {
            break;
        }

        // Double character closing sequences (Multi line comments)
        else if(commType == '*' && last == '*' && b[i] == '/') {
            ++i;
            break;
        }

        // Normal characters (part of the comment)
        else {
            last = *b[i];
            ++i;
        }
    }


    return i - index;
}








/**
 * @brief Calculates the length of the string or char literal that strarts at index <index> and saves its characters in <r>.
 * @param b The buffer that contains the literal.
 * @param index The index at which the literal starts.
 * @param r The buffer in which to save the literal.
 * @return The length of the literal, including the opening and closing character sequences, or 0 if none was found.
 */
ulong pre::saveLiteral(SegmentedCleanSource *b, ulong index, SegmentedCleanSource *r) {
    if(b->str[index] != '"' && b->str[index] != '\'') return 0;
    r->str  += *b->str[index];
    r->meta += *b->meta[index];


    char literalType = *b->str[index];
    ulong i = index + 1;
    while(true) {

        // Escape sequences
        if(b->str[i] == '\\') {
            r->str  += *b->str[i];
            r->meta += *b->meta[i];
            if(b->str[i].has_value()) {
                r->str  += *b->str[i + 1];
                r->meta += *b->meta[i + 1];
            }
            i += 2;
        }

        // Closing sequence
        //! Macro definitions are skipped by the startCleanupPhase() function. No need to check
        else if(b->str[i] == literalType) {
            r->str  += *b->str[i];
            r->meta += *b->meta[i];
            ++i;
            break;
        }

        // Missing closing sequence
        else if(!b->str[i].has_value()) {
            ulong lastI = std::min(i - 1, b->str.length() - 1);
            utils::printError(
                literalType == '"' ? ErrorCode::ERROR_CMP_STRING_INCOMPLETE_0 : ErrorCode::ERROR_CMP_CHAR_INCOMPLETE_0,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(b, index, lastI),
                ElmCoords(b, lastI, lastI),
                std::string(literalType == '"' ? "String" : "Char") + " literal is missing a closing " + (literalType == '"' ? "\"" : "'") + " character.",
                true //TODO recovery system. skip to the first token that makes sense
            );
        }
        else if(b->str[i] == '\n') {
            ulong lastI = std::min(i - 1, b->str.length() - 1);
            utils::printError(
                literalType == '"' ? ErrorCode::ERROR_CMP_STRING_INCOMPLETE_n : ErrorCode::ERROR_CMP_CHAR_INCOMPLETE_n,
                utils::ErrType::PREPROCESSOR,
                ElmCoords(b, index, lastI),
                ElmCoords(b, lastI, lastI),
                std::string(literalType == '"' ? "String" : "Char") + " literal is missing a closing " + (literalType == '"' ? "\"" : "'") + " character.\n" +
                "If you wish to include a newline character in the literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\".",
                true //TODO recovery system. skip to the first token that makes sense
            );
        }

        // Normal characters
        else {
            r->str  += *b->str[i];
            r->meta += *b->meta[i];
            ++i;
        }
    }


    return i - index;
}








void pre::startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {

    // Try to execute the subphase
    try {
        __internal_startCleanupPhase(b, r);
        r->str.closePipe();
        r->meta.closePipe();
    }

    // If errors occur, close the return pipes and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->str.closePipe();
        r->meta.closePipe();
        std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Preprocessing_A].totalProgress->setProgressColor(ansi::red); //FIXME change bar color to red if failed
    }
}