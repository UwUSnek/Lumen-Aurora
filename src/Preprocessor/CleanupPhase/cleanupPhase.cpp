#include "ALC.hpp"

#include "cleanupPhase.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"








namespace pre {
    void startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r) {



        ulong i = 0;
        while(b->str[i].has_value()) {

            // Skip comments
            ulong commentLen = measureComment(b->str, i);
            if(commentLen) {
                i += commentLen;
                increaseLocalProgress(commentLen);
                continue;
            }

            // Skip (and preserve) literals
            ulong literalLen = saveLiteral(b, i, r);
            if(literalLen) {
                i += literalLen;
                continue;
            }

            // Skip (and preserve) macro definitions and calls
            // FIXME


            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            //FIXME MOVE TO TOKENIZER
            // if(i == 0 || b.str[i - 1] != '\\') { //FIXME ONLY CHECK THIS IF INSIDE A MACRO DEFINITION
            //     // Parse strings
            //     ulong strLiteralLen = parseStrLiteral(b, i, r);
            //     if(strLiteralLen) {
            //         i += strLiteralLen;
            //         continue;
            //     }

            //     // Parse chars
            //     ulong charLiteralLen = parseCharLiteral(b, i, r);
            //     if(charLiteralLen) {
            //         i += charLiteralLen;
            //         continue;
            //     }
            // }

            // Save normal characters
            r->str  += *b->str[i];
            r->meta += *b->meta[i];
            ++i;
        }


        r->str.closePipe();
        r->meta.closePipe();
    }








//TODO USE THESE IN THE TOKENIZER //FIXME REMOVE LCT AND COMMENT CHECKS FROM WHITESPACE COUNTER
//TODO USE THESE IN THE TOKENIZER //FIXME REMOVE LCT AND COMMENT CHECKS FROM WHITESPACE COUNTER
//TODO USE THESE IN THE TOKENIZER //FIXME REMOVE LCT AND COMMENT CHECKS FROM WHITESPACE COUNTER
    // /**
    //  * @brief Counts the whitespace characters in the string <b>, from the character at index <index> until the first non-whitespace character.
    //  *      Comments are considered whitespace.
    //  *      Line continuation tokens are counted as 2 whitespace characters.
    //  * @param b The string buffer that contains the whitespace.
    //  * @param index The index at which the whitespace starts.
    //  * @param DBG_curLine The current line number in the original file at which the whitespace starts.
    //  * @param DBG_filePath The path to the file the buffer string was read from.
    //  * @return A pair containing the number of whitespace characters found and the number of lines they occupy
    //  */
    // WhitespaceInfo countWhitespaceCharacters(std::string b, ulong index, ulong DBG_curLine, std::string DBG_filePath) {
    //     WhitespaceInfo r;
    //     ulong i = index;
    //     while(true) {
    //         // LCTs
    //         ulong lct = checkLct(b, i);
    //         if(lct) {
    //             ++r.h;
    //             i += lct;
    //         }

    //         // Normal whitespace
    //         else if(b[i] == ' ' || b[i] == '\t') {
    //             ++i;
    //             r.isBreaking = true;
    //         }

    //         // Newlines
    //         else if(b[i] == '\n') {
    //             ++i;
    //             ++r.h;
    //             r.isBreaking = true;
    //         }


    //         else {
    //             // Comments
    //             std::pair<ulong, ulong> commentRes = countCommentCharacters(b, i, DBG_curLine + r.h, DBG_filePath);
    //             if(commentRes.first > 0) {
    //                 i += commentRes.first;
    //                 r.h += commentRes.second;
    //                 r.isBreaking = true;
    //             }

    //             // Stop at non-whitespace characters (includes \0)
    //             else {
    //                 r.w = i - index;
    //                 return r;
    //             }
    //         }
    //     }
    // }
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER









    /**
     * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
     * @param b The string pipe that contains the comment.
     * @param index The index at which the comment starts.
     * @return The length of the comment, including the length of the opening and closing character sequences (not \0 or \n).
     *     If the buffer doesn't contain a comment that starts at index <index>, 0 is returned.
     */
    ulong measureComment(StringPipe &b, ulong index) {
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
    ulong saveLiteral(SegmentedCleanSource *b, ulong index, SegmentedCleanSource *r) {
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
                    literalType == '"' ? ErrorCode::ERROR_STRING_INCOMPLETE_0 : ErrorCode::ERROR_CHAR_INCOMPLETE_0,
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(b, index, lastI),
                    ElmCoords(b, lastI, lastI),
                    std::string(literalType == '"' ? "String" : "Char") + " literal is missing a closing " + (literalType == '"' ? "\"" : "'") + " character."
                );
            }
            else if(b->str[i] == '\n') {
                ulong lastI = std::min(i - 1, b->str.length() - 1);
                utils::printError(
                    literalType == '"' ? ErrorCode::ERROR_STRING_INCOMPLETE_n : ErrorCode::ERROR_CHAR_INCOMPLETE_n,
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(b, index, lastI),
                    ElmCoords(b, lastI, lastI),
                    std::string(literalType == '"' ? "String" : "Char") + " literal is missing a closing " + (literalType == '"' ? "\"" : "'") + " character.\n" +
                    "If you wish to include a newline character in the literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
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


















//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER
//FIXME MOVE TO TOKENIZER


    // /**
    //  * @brief Parses the string literal that strarts at index <index> and ends at the first non-escaped " character.
    //  * @param b The string buffer that contains the string literal.
    //  * @param index The index at which the string literal starts.
    //  * @param r The output buffer in which to save the characters that make up the literal. Opening and closing character sequences ar NOT pushed.
    //  * @return The length of the string literal. 0 if none was found.
    //  */
    // ulong parseStrLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r) {
    //     if(b.str[index] != '"') return 0;
    //     r.str += '"';
    //     r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, b.meta[index].i, b.meta[index].l, b.meta[index].f));




    //     ulong i = index + 1;
    //     while(true) {

    //         // Escape sequences
    //         ulong escapeLen = decodeEscapeSequence(b, i, r, CleanSourceType::STRING);
    //         if(escapeLen) {
    //             i += escapeLen;
    //         }

    //         // Closing sequence (escaped closing sequences are parsed by the previous step)
    //         else if(b.str[i] == '"') {
    //             r.str += '"';
    //             r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, b.meta[i].i, b.meta[i].l, b.meta[i].f));
    //             ++i;
    //             break;
    //         }

    //         // Malformed strings
    //         else if(b.str[i] == '\0') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(b, index, i - 1),
    //                 ElmCoords(b, i - 1, i - 1),
    //                 "String literal is missing a closing '\"' character."
    //             );
    //         }
    //         else if(b.str[i] == '\n') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(b, index, i - 1),
    //                 ElmCoords(b, i - 1, i - 1),
    //                 "String literal is missing a closing '\"' character.\n"
    //                 "If you wish to include a newline character in the literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
    //             );
    //         }

    //         // Normal characters (part of the string)
    //         else {
    //             r.str += b.str[i];
    //             r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, b.meta[i].i, b.meta[i].l, b.meta[i].f));
    //             ++i;
    //         }
    //     }




    //     return i - index;
    // }








    // /**
    //  * @brief Parses the char literal that starts at index <index> and checks if it contains a single 8-bit character (or a valid escape sequence).
    //  * @param b The string buffer that contains the char literal.
    //  * @param index The index at which the char literal starts.
    //  * @param r The buffer in which to push the characters that make up the literal. Opening and closing character sequences ar NOT pushed.
    //  * @return The length of the char literal. 0 if none was found.
    //  */
    // ulong parseCharLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r) {
    //     if(b.str[index] != '\'') return 0;
    //     r.str += '\'';
    //     r.meta.push_back(CleanSourceMeta(CleanSourceType::CHAR, b.meta[index].i, b.meta[index].l, b.meta[index].f));




    //     ulong i = index + 1;
    //     ulong finalLen = 1;  //! Starts from 1 to account for the skipped starting ' character
    //     while(true) {

    //         // Escape sequences
    //         ulong oldRLen = r.str.length();
    //         ulong raw_escapeLen = decodeEscapeSequence(b, i, r, CleanSourceType::CHAR); // The length of the scape sequence as written in the source code
    //         ulong utf_escapeLen = r.str.length() - oldRLen;                             // The length of the UTF-8 representation of the escape sequence
    //         if(raw_escapeLen) {
    //             i += raw_escapeLen;
    //             finalLen += utf_escapeLen;
    //         }

    //         // Closing sequence (escaped closing sequences are parsed by the previous step)
    //         else if(b.str[i] == '\'') {
    //             r.str += '\'';
    //             r.meta.push_back(CleanSourceMeta(CleanSourceType::CHAR, b.meta[i].f, b.meta[i].l, b.meta[i].i));
    //             ++i;
    //             ++finalLen;
    //             break;
    //         }

    //         // Missing closing sequence
    //         else if(b.str[i] == '\0') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(b, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
    //                 ElmCoords(b, i - 1, i - 1),
    //                 "Char literal is missing a closing ' character."
    //             );
    //         }
    //         else if(b.str[i] == '\n') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(b, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
    //                 ElmCoords(b, i - 1, i - 1),
    //                 "Char literal is missing a closing ' character.\n"
    //                 "If you wish to include a newline character in the literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
    //             );
    //         }


    //         // Normal characters
    //         else {
    //             r.str += b.str[i];
    //             r.meta.push_back(CleanSourceMeta(CleanSourceType::CHAR, b.meta[i].f, b.meta[i].l, b.meta[i].i));
    //             ++i;
    //             ++finalLen;
    //         }
    //     }
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS




    //     // // Push plain characters
    //     // if(finalLen == 3) {
    //     //     r.str += b.str[index + 1];
    //     //     r.meta.push_back(CleanSourceMeta(CleanSourceType::CHAR, b.meta[index + 1].i, b.meta[index + 1].l, b.meta[index + 1].f));

    //     //     // If other data is present after the escape sequence, print an error
    //     //     if(b[i] != '\'') {
    //     //         utils::printError(
    //     //             utils::ErrType::PREPROCESSOR,
    //     //             ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, i - 1),
    //     //             ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, i, i),
    //     //             "Char literal contains more than one byte. This is not allowed.\n"
    //     //             "If you wish to store strings or a multi-byte Unicode character, you can use a string literal."
    //     //         );
    //     //     }
    //     // }

    //     // Detect non-escaped character sequences longer than 1 byte
    //     if(finalLen > 3) {
    //         utils::printError(
    //             utils::ErrType::PREPROCESSOR,
    //             ElmCoords(b, index, i - 1),
    //             ElmCoords(b, index + 1, i - 2),
    //             "Char literal contains more than one byte. This is not allowed.\n"
    //             "If you wish to store strings or a multi-byte Unicode character, you can use a string literal."
    //         );
    //     }

    //     // Detect empty literals
    //     if(finalLen == 2) {
    //         utils::printError(
    //             utils::ErrType::PREPROCESSOR,
    //             ElmCoords(b, index, i - 1),
    //             "Char literal cannot be empty."
    //         );
    //     }




    //     return i - index;
    // }








    // //TODO PUT THIS IN THE DOCUMENTATION. these are usable in strings and chars, but longer sequences produce errors when used inside of chars
    // /**
    //  * @brief Parses the escape sequence that starts at index <index> and prints an error if it is invalid.
    //  * @param b The string buffer that contains the escape sequence.
    //  * @param index The index at which the escape sequence starts.
    //  * @param r The buffer in which to push the decoded UTF-8 bytes.
    //  * @param literalType The type to use when pushing characters to <r>.
    //  * @return A pair containing the length and height of the escape sequence, or (0, 0) if none was found.
    //  */
    // ulong decodeEscapeSequence(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r, CleanSourceType literalType) {

    //     // Return if there is no escape sequence
    //     if(b.str[index] != '\\') return 0;


    //     // If there is
    //     ulong i = index + 1, h = 0;
    //     CleanSourceMeta outputCharData(literalType, b.meta[index].i, b.meta[index].l, b.meta[index].f);
    //     //!  ^ Create bogus character data. All the output characters will use a copy of this.
    //     switch(b.str[i]) {

    //         // Convert basic escapes
    //         case '\\': { r.str += '\\'; r.meta.push_back(outputCharData); return 2; }
    //         case '0' : { r.str += '\0'; r.meta.push_back(outputCharData); return 2; }
    //         case '\'': { r.str += '\''; r.meta.push_back(outputCharData); return 2; }
    //         case '"' : { r.str += '\"'; r.meta.push_back(outputCharData); return 2; }
    //         case 'n' : { r.str += '\n'; r.meta.push_back(outputCharData); return 2; }
    //         case 'r' : { r.str += '\r'; r.meta.push_back(outputCharData); return 2; }
    //         case 't' : { r.str += '\t'; r.meta.push_back(outputCharData); return 2; }
    //         case 'v' : { r.str += '\v'; r.meta.push_back(outputCharData); return 2; }


    //         // Convert Unicode codepoint escapes
    //         case 'u': case 'U': {

    //             // Calculate expected digits and skip the u character
    //             ulong expectedDigits = b.str[i] == 'U' ? 8 : 4;
    //             ++i;

    //             // Parse the codepoint
    //             std::string codepoint;
    //             for(ulong j = 0; j < expectedDigits && i + j < b.str.length();) {
    //                 char c = b.str[i + j];
    //                 if(c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f') {
    //                     codepoint += c;
    //                     ++j;
    //                 }
    //                 else break;
    //             }
    //             i += codepoint.length();


    //             // Check bad length
    //             if(codepoint.empty()) {
    //                 utils::printError(
    //                     utils::ErrType::PREPROCESSOR,
    //                     ElmCoords(b, index, index + 1),
    //                     ElmCoords(b, index + 2, i - 1), //FIXME check if this is visible and works well when at the end of the file
    //                     "Missing hexadecimal digits after Unicode codepoint escape sequence.\n"   //FIXME ^ debug it and check that the idices don't segfault
    //                     "Exacly " + std::to_string(expectedDigits) + " digits are required."
    //                 );
    //             }
    //             else if(codepoint.length() < expectedDigits) {
    //                 utils::printError(
    //                     utils::ErrType::PREPROCESSOR,
    //                     ElmCoords(b, index, index + 1),
    //                     ElmCoords(b, index + 2, i - 1), //FIXME CHECK IF THIS WORKS IF AT THE END OF THE FILE (it should)
    //                     "Missing hexadecimal digits in Unicode codepoint escape sequence.\n" +    //FIXME ^ debug it and check that the idices don't segfault
    //                     std::to_string(expectedDigits) + " were expected, but only " + std::to_string(codepoint.length()) + " could be found."
    //                 );
    //             }

    //             // Convert the codepoint
    //             unsigned char utfValue[5];
    //             //TODO check errors from the function
    //             //TODO modify decoder to remove unnecessary checks already performed by the parser
    //             utf8decode((const utf8chr_t*)(codepoint.c_str()), utfValue);

    //             // Push to buffer
    //             for(ulong j = 0; utfValue[j] != '\0'; ++j){
    //                 r.str += utfValue[j];
    //                 r.meta.push_back(outputCharData);
    //             }

    //             // Return length
    //             return i - index;
    //         }


    //         // Print an error for invalid sequences
    //         default: {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(b, index, i - 1),
    //                 "Invalid escape sequence \"" + ansi::white + "\\" + b.str[i] + ansi::reset + "\"."
    //             );

    //             //! return is never executed. printError() stops the program
    //             //! Its only here so GCC doesn't cry about it
    //             return 0;
    //         }
    //     }
    // }
    // // TODO WRITE TESTS FOR EACH OF THESE CASES
    // // TODO WRITE TESTS FOR EACH OF THESE CASES
    // // TODO WRITE TESTS FOR EACH OF THESE CASES
}