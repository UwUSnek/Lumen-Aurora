#include <tuple>
#include "cleanupPhase.hpp"
#include "WhitespaceInfo.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "SegmentedCleanSource.hpp"






namespace pre {
    /**
     * @brief Checks if a Line continuation token is present in the string <b> at the index <index> and calculates its length.
     * @param b The stirng.
     * @param index The index to check.
     * @return The number of characters the LCT occupies, or 0 if one was not found.
     */
    ulong checkLct(std::string b, ulong index) {
        if(b[index] == '\\' && b[index + 1] == '\n') return 2;
        if(b[index] == '\0') return 1;
        return 0;
    }








    /**
     * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
     * @param b The string buffer that contains the comment.
     * @param index The index at which the comment starts.
     * @param DBG_curLine The current line number in the original file at which the comment starts.
     * @param DBG_filePath The path to the file the buffer string was read from.
     * @return A pair containing the length of the comment and the number of additional lines it occupies, including the length of the opening and closing character sequences (including \\n but excluding \0).
     *     If the buffer doesn't contain a comment that starts at index <index>, (0, 0) is returned.
     */
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DBG_filePathIndex) {
        if(b[index] != '/') return std::pair<ulong, ulong>(0, 0);


        char last = b[index];
        char commType = '\0'; // '\0' if unknow, '/' if single line, '*' if multiline
        ulong i = index + 1, h = 0;
        while(true) {
            // Check line continuation token
            ulong lct = checkLct(b, i);
            if(lct) {
                ++h;
                i += lct;
            }

            // Starting sequence
            else if(commType == '\0') {
                if(b[i] == '/' || b[i] == '*') {
                    commType = b[i];
                    continue;
                }
                else {  //! Starting sequence not found (this includes \n and \0 cases)
                    return std::pair<ulong, ulong>(0, 0);
                }
            }

            // Single character closing sequences (End of file or single line comments)
            else if(b[i] == '\0' || commType == '/' && b[i] == '\n') {
                break;
            }

            // Double character closing sequences (Multi line comments)
            else if(commType == '*' && last == '*' && b[i] == '/') {
                ++i;
                break;
            }

            // Normal characters (part of the comment)
            else {
                if(b[i] == '\n') ++h;
                last = b[i];
                ++i;
            }
        }


        return std::pair<ulong, ulong>(i - index, h);
    }








    //FIXME use a stream and process the steps concurrently
    SegmentedCleanSource startCleanupPhase(std::string b, ulong DBG_filePathIndex) {
        SegmentedCleanSource r;


        ulong i = 0;                // Current index relative to the raw data
        ulong curLine = 0;          // The current line number relative to the raw data
        while(i < b.length()) {

            // Skip LCTs
            ulong lct = checkLct(b, i);
            if(lct) {
                i += lct;
                ++curLine;
                continue;
            }

            // Skip comments
            std::pair<ulong, ulong> comment = countCommentCharacters(b, i, DBG_filePathIndex);
            if(comment.first > 0) {
                i += comment.first;
                curLine += comment.second;
            }

            // Save normal characters
            else {
                r.str += b[i];
                r.og.push_back(CleanSourceData(i, curLine, DBG_filePathIndex));
                if(b[i] == '\n') ++curLine;
                ++i;
            }
        }


        return r;
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









    // //TODO check if multi-byte unicode characters can screw with the string detection
    // /**
    //  * @brief Parses the string literal that strarts at index <index> and ends at the first non-escaped " character.
    //  * @param b The string buffer that contains the string literal.
    //  * @param index The index at which the string literal starts.
    //  * @param DBG_curLine The current line number in the original file at which the string literal starts.
    //  * @param DBG_filePath The path to the file the buffer string was read from.
    //  * @return Informations about the string literal, including the two " characters
    //  *     If the buffer doesn't contain a string literal that starts at index <index>, a result with type NONE is returned.
    //  */
    // ParsingResult parseStrLiteral(std::string b, ulong index, ulong DBG_curLine, std::string DBG_filePath) {
    //     ParsingResult r;
    //     if(b[index] != '"') return r;
    //     r.trueValue  += '"';
    //     r.finalValue += '"';


    //     char last = b[index];
    //     ulong i = index + 1;
    //     while(true) {
    //         // Check line continuation token
    //         ulong lct = checkLct(b, i);
    //         if(lct) {
    //             r.trueValue += b.substr(i, lct);
    //             ++r.height;
    //             i += lct;
    //         }

    //         // Malformed strings
    //         else if(b[i] == '\0') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DBG_curLine + r.height, index, i),
    //                 "String literal is missing a closing '\"' character."
    //             );
    //         }
    //         else if(b[i] == '\n') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DBG_curLine + r.height, index, i),
    //                 "String literal is missing a closing '\"' character.\n"
    //                 "If you wish to include a newline character in the string, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
    //             );
    //         }

    //         // Closing sequence
    //         else if(last != '\\' && b[i] == '"') {
    //             r.trueValue  += '"';
    //             r.finalValue += '"';
    //             break;
    //         }

    //         // Normal characters (part of the string)
    //         else {
    //             r.trueValue  += b[i];
    //             r.finalValue += b[i];
    //             last = b[i];
    //             ++i;
    //         }
    //     }

    //     r.elmType = ICF_ElmType::STRING;
    //     return r;
    // }








    // /**
    //  * @brief Parses the char literal that starts at index <index> and checks if it contains a single 8-bit character (or a valid escape sequence).
    //  * @param b The string buffer that contains the char literal.
    //  * @param index The index at which the char literal starts.
    //  * @param DBG_curLine The current line number in the original file at which the char literal starts.
    //  * @param DBG_filePath The path to the file the buffer string was read from.
    //  * @return Informations about the char literal, including the two ' characters.
    //  *     If the buffer doesn't contain a char literal that starts at index <index>, a result with type NONE is returned.
    //  */
    // ParsingResult parseCharLiteral(std::string b, ulong index, ulong DBG_curLine, std::string DBG_filePath) {
    //     ParsingResult r;
    //     if(b[index] != '\'') return r;
    //     r.trueValue  += '\'';
    //     r.finalValue += '\'';


    //     // Check for empty char literals or unescaped ' characters

    //     //TODO check if the actual character is valid. \n \0 and others are not valid. they need to be escaped
    //     //TODO Print a special error if they are actually used raw. tell the user to escape them
    //     //TODO



    //     char last = b[index];
    //     ulong i = index + 1;
    //     while(true) {
    //         // Check line continuation token
    //         ulong lct = checkLct(b, i);
    //         if(lct) {
    //             r.trueValue += b.substr(i, i + lct);
    //             ++r.height;
    //             i += lct;
    //         }

    //         // Closing sequence
    //         else if(last != '\\' && b[i] == '\'') {
    //             r.trueValue  += '\'';
    //             r.finalValue += '\'';
    //             break;
    //         }

    //         // Missing closing sequence
    //         else if(b[i] == '\0') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DBG_curLine + r.height, index, i),
    //                 "Char literal is missing a closing ' character."
    //             );
    //         }
    //         else if(b[i] == '\n') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DBG_curLine + r.height, index, i),
    //                 "Char literal is missing a closing ' character.\n"
    //                 "If you wish to use a newline character in the char literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
    //             );
    //         }

    //         // Normal characters and escape sequences
    //         else {
    //             r.trueValue  += b[i];
    //             r.finalValue += b[i];
    //             last = b[i];
    //             ++i;
    //         }
    //     }




    //     ulong finalLen = r.finalValue.length();
    //     if(finalLen > 4 || finalLen == 4 && r.finalValue[1] != '\\') {
    //         utils::printError(
    //             utils::ErrType::PREPROCESSOR,
    //             ElmCoords(DBG_filePath, DBG_curLine + r.height, index, i),
    //             "Char literal contains more than one byte. This is not allowed.\n"
    //             "If you wish to store strings or a multi-byte Unicode character, you can use a string literal."
    //         );
    //     }
    //     else if(r.finalValue.length() == 4) { //!  && r.finalValue[1] == '\\'
    //         //FIXME check valid escape sequences
    //     }
    //     else if(r.finalValue.length() == 3) {
    //         if(r.finalValue[1] == '\\') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DBG_curLine + r.height, index, i),
    //                 "Char literal is missing a closing ' character.\n"
    //                 "Did you mean " + ansi::bold_cyan + "'\\'" + ansi::reset + "?"
    //             );
    //         }
    //         //! Else return;
    //     }
    //     else if(r.finalValue.length() == 2) {
    //         utils::printError(
    //             utils::ErrType::PREPROCESSOR,
    //             ElmCoords(DBG_filePath, DBG_curLine, index, i),
    //             "Char literal cannot be empty."
    //         );
    //     }



    //     r.elmType = ICF_ElmType::CHAR;
    //     return r;
    // }
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
}