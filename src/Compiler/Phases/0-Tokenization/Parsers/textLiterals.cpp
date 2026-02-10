#include "textLiterals.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ansi.hpp"
#include "Main/errors.hpp"










extern "C" {
    #include "Compiler/UTF8_decoder.h"
}








/**
 * @brief Returns the name of the literal type as a string.
 * @param literalType The type of literal.
 * @return The name. Either "String" or "Char"
 */
const char* getLiteralName(const cmp::TextLiteralType literalType) {
    return literalType == cmp::TextLiteralType::STRING ? "String" : "Char";
}








//TODO PUT THIS IN THE DOCUMENTATION. these are usable in strings and chars, but longer sequences produce errors when used inside of chars
/**
 * @brief Parses the escape sequence that starts at index <index> and prints an error if it is invalid.
 * @param b The buffer that contains the escape sequence.
 * @param index The index at which the escape sequence starts.
 * @param rawEscapeLen The raw length of the escape sequence (the number of characters it occupies in the original source code)
 * @param literalType The type of literal this escape sequence is part of. This affects error messages.
 * @return The string value of the translated escape sequence, expressed in the UTF-8 format, or nullopt if one was not found.
 */
std::optional<std::string> decodeEscapeSequence(ptr<pre::SegmentedCleanSource> b, ulong index, ulong *rawEscapeLen, cmp::TextLiteralType literalType) {
    std::stringstream r;

    // Return if there is no escape sequence
    if(const auto &c0 = b->str[index]; !c0.has_value() || c0 != '\\') {
        *rawEscapeLen = 0;
        return std::nullopt;
    }


    // If there is
    ulong i = index + 1;
    if(const auto &c = b->str[i]; c.has_value()) switch(*c) {


        // Invalid escape sequences
        default: {
            utils::printError(
                ErrorCode::ERROR_CMP_ESCAPE_INVALID,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                "Invalid escape sequence \"" + ansi::white + "\\" + *c + ansi::reset + "\".",
                true //TODO recovery system. skip to the first token that makes sense
            );
        }


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
                    ErrorCode::ERROR_CMP_ESCAPE_U_EMPTY,
                    utils::ErrType::COMPILER,
                    ElmCoords(b, index, index + 1),
                    ElmCoords(b, index + 2, i - 1), //FIXME check if this is visible and works well when at the end of the file //FIXME ^ debug it and check that the idices don't segfault
                    std::format(
                        "Missing hexadecimal digits after Unicode codepoint escape sequence.\nExactly {} digits are required.",
                        expectedDigits
                    ),
                    true //TODO recovery system. skip to the first token that makes sense
                );
            }
            else if(codepoint.length() < expectedDigits) {
                utils::printError(
                    *c == 'u' ? ErrorCode::ERROR_CMP_ESCAPE_U4_MISSING : ErrorCode::ERROR_CMP_ESCAPE_U8_MISSING,
                    utils::ErrType::COMPILER,
                    ElmCoords(b, index, index + 1),
                    ElmCoords(b, index + 2, i - 1), //FIXME CHECK IF THIS WORKS IF AT THE END OF THE FILE (it should) //FIXME ^ debug it and check that the idices don't segfault
                    std::format(
                        "Missing hexadecimal digits in Unicode codepoint escape sequence.\n{} were expected, but only {} could be found.",
                        expectedDigits,
                        codepoint.length()
                    ),
                    true //TODO recovery system. skip to the first token that makes sense
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


    // Print an error for unexpected end of file
    else {
        utils::printError(
            ErrorCode::ERROR_CMP_UNEXPECTED_END,
            utils::ErrType::COMPILER,
            ElmCoords(b, index, i - 1),
            std::format("Unexpected end of file in escape sequence of {} literal.", getLiteralName(literalType)),
            true //TODO recovery system. skip to the first token that makes sense
        );
    }


    //! return is never executed. printError() stops the program
    //! Its only here so GCC doesn't cry about it
    return std::nullopt;
}








/**
 * @brief Parses and validates the text literal (char or string) that starts at index <index>.
 * @param b The buffer that contains the literal.
 * @param index The index at which the literal starts.
 * @param rawLiteralLen The raw length of the literal (the number of characters it occupies in the original source code)
 * @return The string value of the literal, or nullopt if one was not found.
 */
ptr<cmp::TokenValue> cmp::parseTextLiteral(ptr<pre::SegmentedCleanSource> b, ulong index, ulong *rawLiteralLen, cmp::TextLiteralType literalType) {
    std::stringstream r;
    const char delimiter = literalType == cmp::TextLiteralType::STRING ? '"' : '\'';
    if(const auto &c0 = b->str[index]; !c0.has_value() || c0 != delimiter) {
        *rawLiteralLen = 0;
        return nullptr;
    }


//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
//FIXME merge with string logic. they are ideantical but use different names and delimiters
    ulong i = index + 1;
    while(true) {
        std::optional<char> c = b->str[i];


        // Missing closing sequence
        if(!c.has_value()) {
            utils::printError(
                ErrorCode::ERROR_CMP_STRING_INCOMPLETE_0,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
                ElmCoords(b, i - 1, i - 1),
                std::format("{} literal is missing a closing {} character.", getLiteralName(literalType), delimiter),
                true //TODO recovery system. skip to the first token that makes sense
            );
        }
        else if(c == '\n') {
            utils::printError(
                ErrorCode::ERROR_CMP_STRING_INCOMPLETE_n,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),//FIXME CHECK IF '' and 'a' AT THE END OF THE FILE ARE DETECTED AND SHOWN CORRECTLY
                ElmCoords(b, i - 1, i - 1),
                std::format(
                    "{} literal is missing a closing {} character.\n"
                    "If you wish to include a newline character in the literal, use the escape sequence \"{}\".",
                    getLiteralName(literalType), delimiter,
                    ansi::bold_cyan + "\\n" + ansi::reset
                ),
                true //TODO recovery system. skip to the first token that makes sense
            );
        }

        // Escape sequences
        // ulong oldRLen = r.tellp();
        ulong rawEscapeLen;
        // // The length of the scape sequence as written in the source code
        // ulong utf_escapeLen = r.str.length() - oldRLen;                             // The length of the UTF-8 representation of the escape sequence
        if(const auto &decodedEscape = decodeEscapeSequence(b, i, &rawEscapeLen, literalType); decodedEscape.has_value()) {
            r << *decodedEscape;
            i += rawEscapeLen;
            continue;
        }

        // Closing sequence (escaped closing sequences are parsed by the previous escape sequence step)
        if(c == delimiter) {
            ++i;
            break;
        }

        // Normal characters
        r << *c;
        ++i;
    }
    //FIXME TEST ALL ESCAPE SEQUENCES AND LOCATIONS




    // For character literals only
    if(literalType == cmp::TextLiteralType::CHAR) {

        // Detect non-escaped character sequences longer than 1 byte
        if(r.tellp() > 1) {
            utils::printError(
                ErrorCode::ERROR_CMP_CHAR_LONG,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                ElmCoords(b, index + 1, i - 2),
                "Char literal contains more than one byte. This is not allowed.\n"
                "If you wish to store strings or a multi-byte Unicode character, you can use a string literal.",
                true //TODO recovery system. skip to the first token that makes sense
            );
        }

        // Detect empty literals
        if(r.tellp() == 0) {
            utils::printError(
                ErrorCode::ERROR_CMP_CHAR_EMPTY,
                utils::ErrType::COMPILER,
                ElmCoords(b, index, i - 1),
                "Char literal cannot be empty.",
                true //TODO recovery system. skip to the first token that makes sense
            );
        }
    }




    // Calculate raw literal length and return the token
    *rawLiteralLen = i - index;
    if(literalType == cmp::TextLiteralType::STRING) {
        std::string rStr = r.str();
        return newptr<TK_String>(rStr.substr(1, rStr.length() - 2));
    }
    else {
        return newptr<TK_Char>(r.str()[0]);
    }
}