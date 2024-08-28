#include <tuple>
#include <regex>

#include "cleanupPhase.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "UTF8_decoder.h"




//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE
//FIXME REMOVE LCTS CHECKS FROM THIS PHASE



namespace pre {
    //FIXME use a stream and process the steps concurrently
    SegmentedCleanSource startCleanupPhase(SegmentedCleanSource &b) {
        SegmentedCleanSource r;


        ulong i = 0;  // Current index relative to the raw data
        while(i < b.str.length()) {
            // Skip comments
            ulong commentLen = measureComment(b.str, i);
            if(commentLen > 0) {
                i += commentLen;
            }


            if(i == 0 || b.str[i - 1] != '\\') { //FIXME ONLY CHECK THIS IF INSIDE A MACRO DEFINITION
                // Parse strings
                //FIXME ADD ESCAPE SEQUENCE PARSING
                //FIXME ADD ESCAPE SEQUENCE PARSING
                //FIXME ADD ESCAPE SEQUENCE PARSING
                ulong strLiteralLen = parseStrLiteral(b, i, r);
                if(strLiteralLen) {
                    i += strLiteralLen;
                    continue;
                }

                //FIXME
                //FIXME
                //FIXME
                //FIXME
                // // Parse chars
                // std::pair<ulong, ulong> charLiteral = parseCharLiteral(b, i, curLine, DBG_filePathIndex, r);
                // if(charLiteral.first) {
                //     i += charLiteral.first;
                //     curLine += charLiteral.second;
                //     continue;
                // }
            }

            // Save normal characters
            r.str += b.str[i];
            // r.meta.push_back(CleanSourceMeta(CleanSourceType::MISC, i, curLine, DBG_filePathIndex));
            r.meta.push_back(b.meta[i]);
            ++i;
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
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER
//TODO USE THESE IN THE TOKENIZER









    /**
     * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
     * @param b The string buffer that contains the comment.
     * @param index The index at which the comment starts.
     * @param DBG_curLine The current line number in the original file at which the comment starts.
     * @param DBG_filePath The path to the file the buffer string was read from.
     * @return The length of the comment, including the length of the opening and closing character sequences (not \0 or \n).
     *     If the buffer doesn't contain a comment that starts at index <index>, 0 is returned.
     */
    ulong measureComment(std::string &b, ulong index) {
        if(b[index] != '/') return 0;


        char last = b[index];
        char commType = '\0'; // '\0' if unknow, '/' if single line, '*' if multiline
        ulong i = index + 1;
        while(true) {

            // Starting sequence
            if(commType == '\0') {
                if(b[i] == '/' || b[i] == '*') {
                    commType = b[i];
                    continue;
                }
                else {  //! Starting sequence not found (this includes \n and \0 cases)
                    return 0;
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
                last = b[i];
                ++i;
            }
        }


        return i - index;
    }








    /**
     * @brief Parses the string literal that strarts at index <index> and ends at the first non-escaped " character.
     * @param b The string buffer that contains the string literal.
     * @param index The index at which the string literal starts.
     * @param DBG_curLine The current line number in the original file at which the string literal starts.
     * @param DBG_filePath The path to the file the buffer string was read from.
     * @param r The output buffer in which to save the characters that make up the literal. Opening and closing character sequences ar NOT pushed.
     * @return The length and height of the string literal. 0 if none was found.
     */
    ulong parseStrLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r) {
        if(b.str[index] != '"') return 0;
        r.str += '"';
        r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, b.meta[index].i, b.meta[index].l, b.meta[index].f));
        ulong i = index + 1;


        char last = b.str[index];
        while(true) {

            // Malformed strings
            if(b.str[i] == '\0') {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(sourceFilePaths[b.meta[index].f], b.meta[index].l, b.meta[index].i, b.meta[i - 1].i),
                    ElmCoords(sourceFilePaths[b.meta[i - 1].f], b.meta[i - 1].l, b.meta[i - 1].i, b.meta[i - 1].i),
                    "String literal is missing a closing '\"' character."
                );
            }
            else if(b.str[i] == '\n') {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(sourceFilePaths[b.meta[index].f], b.meta[index].l, b.meta[index].i, b.meta[i - 1].i),
                    ElmCoords(sourceFilePaths[b.meta[i - 1].f], b.meta[i - 1].l, b.meta[i - 1].i, b.meta[i - 1].i),
                    "String literal is missing a closing '\"' character.\n"
                    "If you wish to include a newline character in the string, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
                );
            }

            // Closing sequence
            else if(last != '\\' && b.str[i] == '"') {
                r.str += '"';
                r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, b.meta[i].i, b.meta[i].l, b.meta[i].f));
                ++i;
                break;
            }

            // Normal characters (part of the string)
            else {
                last = b.str[i];
                r.str += last;
                r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, b.meta[i].i, b.meta[i].l, b.meta[i].f));
                ++i;
            }
        }

        return i - index;
    }








    // /**
    //  * @brief Parses the char literal that starts at index <index> and checks if it contains a single 8-bit character (or a valid escape sequence).
    //  * @param b The string buffer that contains the char literal.
    //  * @param index The index at which the char literal starts.
    //  * @param DBG_curLine The current line number in the original file at which the char literal starts.
    //  * @param DBG_filePath The path to the file the buffer string was read from.
    //  * @param r The buffer in which to push the characters that make up the literal. Opening and closing character sequences ar NOT pushed.
    //  * @return A pair containing the length and height of the char literal. (0, 0) if none was found.
    //  */
    // std::pair<ulong, ulong> parseCharLiteral(SegmentedCleanSource &b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource &r) {
    //     if(b[index] != '\'') return std::pair<ulong, ulong>(0, 0);
    //     r.str += '\'';
    //     r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, index, DBG_curLine, DBG_filePathIndex));
    //     ulong i = index + 1, h = 0;


    //     // Check for empty char literals or unescaped ' characters

    //     //TODO check if the actual character is valid. \n \0 and others are not valid. they need to be escaped
    //     //TODO Print a special error if they are actually used raw. tell the user to escape them
    //     //TODO



    //     char last = b[index];
    //     ulong finalLen = 1;  //! Starts from 1 to account for the starting ' character
    //     while(true) {
    //         // Check line continuation token
    //         ulong lct = checkLct(b, i);
    //         if(lct) {
    //             ++h;
    //             i += lct;
    //         }

    //         // Closing sequence
    //         else if(last != '\\' && b[i] == '\'') {
    //             ++i;
    //             ++finalLen;
    //             break;
    //         }

    //         // Missing closing sequence
    //         else if(b[i] == '\0') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine + h, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine + h, i, i),
    //                 "Char literal is missing a closing ' character."
    //             );
    //         }
    //         else if(b[i] == '\n') {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine + h, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine + h, i, i),
    //                 "Char literal is missing a closing ' character.\n"
    //                 "If you wish to use a newline character in the char literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
    //             );
    //         }

    //         // Normal characters and escape sequences
    //         else {
    //             last = b[i];
    //             // r.str += last; //TODO remove comment if unused
    //             // r.og.push_back(CleanSourceData(CleanSourceType::STRING, i, DBG_curLine + h, DBG_filePathIndex));
    //             ++i;
    //             ++finalLen;
    //         }
    //     }
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //     //FIXME ADD ESCAPE PARSING TO STRING FUNCTION

    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS
    //     //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS




    //     // If the character is expressed as an escape sequence, push its decoded characters
    //     std::pair<ulong, ulong> escapeRes = decodeEscapeSequence(b, index + 1, DBG_curLine + h, DBG_filePathIndex, r);
    //     if(escapeRes.first > 0) {
    //         i += escapeRes.first;
    //         h += escapeRes.second;
    //     }


    //     // If not
    //     else {

    //         // Push plain characters
    //         if(finalLen == 3) {
    //             r.str += b[index + 1];
    //             r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, index + 1, DBG_curLine + h, DBG_filePathIndex));

    //             // If other data is present after the escape sequence, print an error
    //             if(b[i] != '\'') {
    //                 utils::printError(
    //                     utils::ErrType::PREPROCESSOR,
    //                     ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, i - 1),
    //                     ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, i, i),
    //                     "Char literal contains more than one byte. This is not allowed.\n"
    //                     "If you wish to store strings or a multi-byte Unicode character, you can use a string literal."
    //                 );
    //             }
    //         }

    //         // Detect non-escaped character sequences longer than 1 byte
    //         else if(finalLen > 3) {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, i - 1),
    //                 "Char literal contains more than one byte. This is not allowed.\n"
    //                 "If you wish to store strings or a multi-byte Unicode character, you can use a string literal."
    //             );
    //         }

    //         // Detect empty literals
    //         else if(finalLen == 2) {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, i - 1),
    //                 "Char literal cannot be empty."
    //             );
    //         }
    //     }




    //     // Push terminator and return
    //     r.str += '"';
    //     r.meta.push_back(CleanSourceMeta(CleanSourceType::STRING, i, DBG_curLine + h, DBG_filePathIndex));
    //     return std::pair<ulong, ulong>(i - index, h);
    // }








    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //FIXME CHECKS IF THIS NEEDS TO CONSIDER LCTs. FIX IT IF IT DOES
    // //TODO PUT THIS IN THE DOCUMENTATION. these are usable in strings and chars, but longer sequences produce errors when used inside of chars
    // /**
    //  * @brief Parses the escape sequence that starts at index <index> and prints an error if it is invalid.
    //  * @param b The string buffer that contains the escape sequence.
    //  * @param index The index at which the escape sequence starts.
    //  * @param DBG_curLine The current line number in the original file at which the char literal starts.
    //  * @param DBG_filePath The path to the file the buffer string was read from.
    //  * @param r The buffer in which to push the decoded UTF-8 bytes.
    //  * @return A pair containing the length and height of the escape sequence, or (0, 0) if none was found.
    //  */
    // std::pair<ulong, ulong> decodeEscapeSequence(SegmentedCleanSource &b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource &r) {

    //     // Return if there is no escape sequence
    //     if(b[index] != '\\') return std::pair<ulong, ulong>(0, 0);


    //     // If there is
    //     ulong i = index + 1, h = 0;
    //     CleanSourceMeta outputCharData(CleanSourceType::STRING, index, DBG_curLine, DBG_filePathIndex);    // Create bogus character data. All the output characters will use a copy of this.
    //     switch(b[i]) {

    //         // Convert basic escapes
    //         //! Newlines (LCTs) that would be in this specific index are parsed out by the string parser before calling this function.
    //         //! No need to check for them.
    //         case '\\': { r.str += '\\'; r.meta.push_back(outputCharData); break; }
    //         case '0' : { r.str += '\0'; r.meta.push_back(outputCharData); break; }
    //         case '\'': { r.str += '\''; r.meta.push_back(outputCharData); break; }
    //         case '"' : { r.str += '\"'; r.meta.push_back(outputCharData); break; }
    //         case 'n' : { r.str += '\n'; r.meta.push_back(outputCharData); break; }
    //         case 'r' : { r.str += '\r'; r.meta.push_back(outputCharData); break; }
    //         case 't' : { r.str += '\t'; r.meta.push_back(outputCharData); break; }
    //         case 'v' : { r.str += '\v'; r.meta.push_back(outputCharData); break; }


    //         // Convert Unicode codepoint escapes
    //         case 'u': case 'U': {

    //             // Parse the codepoint
    //             ulong expectedDigits = b[i] == 'U' ? 8 : 4;
    //             ++i; //! Skip u character
    //             std::string codepoint;
    //             for(ulong j = 0; j < expectedDigits && i + j < b.length();) {
    //                 ulong lct = checkLct(b, i + j);
    //                 if(lct) {
    //                     i += lct;
    //                     ++h;
    //                 }
    //                 else {
    //                     char c = b[j];
    //                     if(c >= '0' && c <= '9' || c >= 'A' && c <= 'F' || c >= 'a' && c <= 'f') {
    //                         codepoint += c;
    //                         ++j;
    //                     }
    //                     else break;
    //                 }
    //             }
    //             i += codepoint.length();

    //             // Check bad length
    //             if(codepoint.empty()) {
    //                 utils::printError(
    //                     utils::ErrType::PREPROCESSOR,
    //                     ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, index + 1),
    //                     ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index + 2, i), //FIXME check if this is visible and works well when at the end of the file
    //                     "Missing hexadecimal digits after Unicode codepoint escape sequence.\n"   //FIXME ^ debug it and check that the idices don't segfault
    //                     "Exacly " + std::to_string(expectedDigits) + " digits are required."
    //                 );
    //             }
    //             else if(codepoint.length() < expectedDigits) {
    //                 utils::printError(
    //                     utils::ErrType::PREPROCESSOR,
    //                     ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, index + 1),
    //                     ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index + 2, i), //FIXME CHECK IF THIS WORKS IF AT THE END OF THE FILE (it should)
    //                     "Missing hexadecimal digits in Unicode codepoint escape sequence.\n" +    //FIXME ^ debug it and check that the idices don't segfault
    //                     std::to_string(expectedDigits) + " were expected, but only " + std::to_string(codepoint.length()) + " could be found."
    //                 );
    //             }

    //             // Convert the codepoint
    //             unsigned char dest[5];
    //             //TODO check errors from the function
    //             //TODO modify decoder to remove unnecessary checks already performed by the parser
    //             utf8decode((const utf8chr_t*)(codepoint.c_str()), dest);

    //             // Push to buffer
    //             for(ulong j = 0; dest[j] != '\0'; ++j){
    //                 r.str += dest[j];
    //                 r.meta.push_back(outputCharData);
    //             }
    //         }


    //         // Print an error for invalid sequences
    //         default: {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(sourceFilePaths[DBG_filePathIndex], DBG_curLine, index, i),
    //                 "Invalid escape sequence \"" + ansi::white + "\\" + b[i] + ansi::reset + "\"."
    //             );
    //         }
    //     }

    //     return std::pair<ulong, ulong>(i - index, h);
    // }
    //TODO WRITE TESTS FOR EACH OF THESE CASES
    //TODO WRITE TESTS FOR EACH OF THESE CASES
    //TODO WRITE TESTS FOR EACH OF THESE CASES
}