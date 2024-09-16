#include <sstream>
#include "literalParser.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"
#include "Compiler/UTF8_decoder.h"








/**
 * @brief Parses the string literal that strarts at index <index> and ends at the first non-escaped " character.
 * @param b The string buffer that contains the string literal.
 * @param index The index at which the string literal starts.
 * @param r The output buffer in which to save the characters that make up the literal. Opening and closing character sequences ar NOT pushed.
 * @return The length of the string literal. 0 if none was found.
 */
//FIXME update documentation
std::optional<std::string> cmp::parseStrLiteral(pre::SegmentedCleanSource *b, ulong index) {
    std::stringstream r;
    std::optional<char> const &c0 = b->str[index];
    if(!c0.has_value() || c0 != '"') return std::nullopt;
    r << '"';




    ulong i = index + 1;
    while(true) {
        std::optional<char> const &c = b->str[i];


        // Malformed strings
        if(!c.has_value()) {
            utils::printError(
                ERROR_CMP_STRING_INCOMPLETE_0,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "String literal is missing a closing '\"' character."
            );
        }
        else if(c == '\n') {
            utils::printError(
                ERROR_CMP_STRING_INCOMPLETE_n,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, i - 1, i - 1),
                "String literal is missing a closing '\"' character.\n"
                "If you wish to include a newline character in the literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
            );
        }

        // Escape sequences
        ulong rawEscapeLen;
        std::optional<std::string> const &decodedEscape = decodeEscapeSequence(b, i, '"', &rawEscapeLen);
        if(decodedEscape.has_value()) {
            r << *decodedEscape;
            i += rawEscapeLen;
            continue;
        }

        // Closing sequence (escaped closing sequences are parsed by the previous step)
        else if(c == '"') {
            r << '"';
            ++i;
            break;
        }

        // Normal characters (part of the string)
        r << *c;
        ++i;
    }




    return r.str();
}








/**
 * @brief Parses the char literal that starts at index <index> and checks if it contains a single 8-bit character (or a valid escape sequence).
 * @param b The string buffer that contains the char literal.
 * @param index The index at which the char literal starts.
 * @param r The buffer in which to push the characters that make up the literal. Opening and closing character sequences ar NOT pushed.
 * @return The length of the char literal. 0 if none was found.
 */
//FIXME update documentation
std::optional<std::string> cmp::parseCharLiteral(pre::SegmentedCleanSource *b, ulong index) {
    std::stringstream r;
    std::optional<char> c0 = b->str[index];
    if(!c0.has_value() || c0 != '\'') return std::nullopt;
    r << '\'';




//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
    ulong i = index + 1;
    ulong finalLiteralLen = 1;  //! Starts from 1 to account for the skipped starting ' character
    while(true) {
        std::optional<char> c = b->str[i];


        // Missing closing sequence
        if(!c.has_value()) {
            utils::printError(
                ERROR_CMP_STRING_INCOMPLETE_0,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
                ElmCoords(b, i - 1, i - 1),
                "Char literal is missing a closing ' character."
            );
        }
        else if(c == '\n') {
            utils::printError(
                ERROR_CMP_STRING_INCOMPLETE_n,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
                ElmCoords(b, i - 1, i - 1),
                "Char literal is missing a closing ' character.\n"
                "If you wish to include a newline character in the literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
            );
        }

        // Escape sequences
        // ulong oldRLen = r.tellp();
        ulong rawEscapeLen;
        std::optional<std::string> const &decodedEscape = decodeEscapeSequence(b, i, '\'', &rawEscapeLen);
        // // The length of the scape sequence as written in the source code
        // ulong utf_escapeLen = r.str.length() - oldRLen;                             // The length of the UTF-8 representation of the escape sequence
        if(decodedEscape.has_value()) {
            r << *decodedEscape;
            i += rawEscapeLen;
            finalLiteralLen += decodedEscape->length();
            continue;
        }

        // Closing sequence (escaped closing sequences are parsed by the previous escape sequence step)
        if(c == '\'') {
            r << '\'';
            ++i;
            ++finalLiteralLen;
            break;
        }

        // Normal characters
        r << *c;
        ++i;
        ++finalLiteralLen;
    }
    //FIXME ADD ESCAPE PARSING TO STRING FUNCTION
    //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS

//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters




    // Detect non-escaped character sequences longer than 1 byte
    if(finalLiteralLen > 3) {
        utils::printError(
            ERROR_CMP_CHAR_LONG,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            ElmCoords(b, index + 1, i - 2),
            "Char literal contains more than one byte. This is not allowed.\n"
            "If you wish to store strings or a multi-byte Unicode character, you can use a string literal."
        );
    }

    // Detect empty literals
    if(finalLiteralLen == 2) {
        utils::printError(
            ERROR_CMP_CHAR_EMPTY,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            "Char literal cannot be empty."
        );
    }




    return r.str();
}








//TODO PUT THIS IN THE DOCUMENTATION. these are usable in strings and chars, but longer sequences produce errors when used inside of chars
/**
 * @brief Parses the escape sequence that starts at index <index> and prints an error if it is invalid.
 * @param b The string buffer that contains the escape sequence.
 * @param index The index at which the escape sequence starts.
 * @param r The buffer in which to push the decoded UTF-8 bytes.
 * @param literalType The type to use when pushing characters to <r>.
 * @return A pair containing the length and height of the escape sequence, or (0, 0) if none was found.
 */
std::optional<std::string> cmp::decodeEscapeSequence(pre::SegmentedCleanSource *b, ulong index, char literalType, ulong *rawEscapeLen) {
    std::stringstream r;

    // Return if there is no escape sequence
    std::optional<char> const &c0 = b->str[index];
    if(!c0.has_value() || c0 != '\\') {
        *rawEscapeLen = 0;
        return std::nullopt;
    }


    // If there is
    ulong i = index + 1;
    std::optional<char> c = b->str[i];
    // pre::CleanSourceMeta outputCharData(literalType, b->meta[index]->i, b->meta[index]->l, b->meta[index]->f);
    // //!  ^ Create bogus character data. All the output characters will use a copy of this.
    if(c.has_value()) switch(*c) {

        // Convert basic escapes
        case '\\': { r << '\\'; *rawEscapeLen = 2; return r.str(); }
        case '0' : { r << '\0'; *rawEscapeLen = 2; return r.str(); }
        case '\'': { r << '\''; *rawEscapeLen = 2; return r.str(); }
        case '"' : { r << '\"'; *rawEscapeLen = 2; return r.str(); }
        case 'n' : { r << '\n'; *rawEscapeLen = 2; return r.str(); }
        case 'r' : { r << '\r'; *rawEscapeLen = 2; return r.str(); }
        case 't' : { r << '\t'; *rawEscapeLen = 2; return r.str(); }
        case 'v' : { r << '\v'; *rawEscapeLen = 2; return r.str(); }


        // Convert Unicode codepoint escapes
        case 'u': case 'U': {

            // Calculate expected digits and skip the u character
            ulong expectedDigits = *c == 'U' ? 8 : 4;
            ++i;

            // Parse the codepoint
            std::string codepoint;
            for(ulong j = 0; j < expectedDigits;) {
                std::optional<char> const &c1 = b->str[i + j];
                if(c1.has_value() && (std::isdigit(*c1) || c1 >= 'A' && c1 <= 'F' || c1 >= 'a' && c1 <= 'f')) {
                    codepoint += *c1;
                    ++j;
                }
                else break;
            }
            i += codepoint.length();


            // Check bad length
            if(codepoint.empty()) {
                utils::printError(
                    ERROR_CMP_ESCAPE_U_EMPTY,
                    utils::ErrType::COMPILER,
                    ElmCoords(b, index, index + 1),
                    ElmCoords(b, index + 2, i - 1), //FIXME check if this is visible and works well when at the end of the file
                    "Missing hexadecimal digits after Unicode codepoint escape sequence.\n"   //FIXME ^ debug it and check that the idices don't segfault
                    "Exacly " + std::to_string(expectedDigits) + " digits are required."
                );
            }
            else if(codepoint.length() < expectedDigits) {
                utils::printError(
                    *c == 'u' ? ERROR_CMP_ESCAPE_U4_MISSING : ERROR_CMP_ESCAPE_U8_MISSING,
                    utils::ErrType::COMPILER,
                    ElmCoords(b, index, index + 1),
                    ElmCoords(b, index + 2, i - 1), //FIXME CHECK IF THIS WORKS IF AT THE END OF THE FILE (it should)
                    "Missing hexadecimal digits in Unicode codepoint escape sequence.\n" +    //FIXME ^ debug it and check that the idices don't segfault
                    std::to_string(expectedDigits) + " were expected, but only " + std::to_string(codepoint.length()) + " could be found."
                );
            }

            // Convert the codepoint
            unsigned char utfValue[5];
            //TODO check errors from the function
            //TODO modify decoder to remove unnecessary checks already performed by the parser
            utf8decode((const utf8chr_t*)(codepoint.c_str()), utfValue);

            // Push to buffer
            for(ulong j = 0; utfValue[j] != '\0'; ++j){
                r << utfValue[j];
            }

            // Set raw length and return the decoded escape sequence
            *rawEscapeLen = i - index;
            return r.str();
        }
    }


    else {
        // Print an error for invalid sequences
        utils::printError(
            ERROR_CMP_ESCAPE_INVALID,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            "Invalid escape sequence \"" + ansi::white + "\\" + *c + ansi::reset + "\"."
        );
    }


    //! return is never executed. printError() stops the program
    //! Its only here so GCC doesn't cry about it
    return std::nullopt;
}
