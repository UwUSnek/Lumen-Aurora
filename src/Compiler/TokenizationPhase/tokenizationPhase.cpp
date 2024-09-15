#include "ALC.hpp"
#include "tokenizationPhase.hpp"
#include "Utils/errors.hpp"








void cmp::startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r) {

    // Copy total progress from preprocessing phase
    phaseDataArrayLock.lock();
    ulong max = phaseDataArray[PhaseID::PREPROCESSING].totalProgress->max.load(); //! Max doesn't change after merging the files
    phaseDataArrayLock.unlock();
    increaseMaxProgress(max);
    //TODO subtract deleted characters from the preprocessor max
    //FIXME include and macro stuff doesnt get counted. fix this
    //FIXME subtract macro calls and definitions
    //FIXME or do this a different way




    ulong i = 0;
    while(b->str[i].has_value()) {


        // Detect whitespace and split tokens if any is found
        ulong wsLen = countWhitespace(b, i);
        if(wsLen) {
            i += wsLen;
            continue;
        }


        // Parse out alphanumeric and symbolic tokens (keywords, meta keywords and identifiers)
        std::optional<std::string> token = parseAlphanumericToken(b, i);
        if(!token.has_value()) token = parseSymbolicToken(b, i);
        if(token.has_value()) {
            TokenType tokenType;

            // If the token is a known keyword
            auto keywordType = keywordMap.find(*token);
            if(keywordType != keywordMap.end()) {
                tokenType = keywordType->second;
            }

            // If not, treat it as an identifier
            else {
                tokenType = TokenType::IDENTIFIER;
            }

            // Push token to output array and update buffer index
            *r += Token(*token, tokenType, *b->meta[i], *b->meta[i + token->length()]);
            i += token->length();
            continue;
        }


        // Print an error if none of the parsers could detect anything (current character is invalid)
        bool debug = b->meta[i].has_value();
        utils::printError(
            ERROR_CMP_CHARACTER_INVALID,
            utils::ErrType::COMPILER,
            ElmCoords(b, i, i),
            std::string("Invalid character '") + *b->str[i] + "'.\n" +
            "This character is not allowed within Lumen or Aurora source code."
        );
        //FIXME ^ decode multi byte characters to print them in the error
    }
}








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








/**
 * @brief Parses the alphanumeric token that starts at index <index> of the buffer <b>.
 *      The token stops at the first character that is not part of its pattern.
 *      [a-zA-Z_][a-zA-Z_0-9]*      //! Alphanumeric identifiers
 *      [a-zA-Z_][a-zA-Z_0-9]*      //! All alphanumeric tokens [what this function checks, includes alphanumeric identifiers]
 * @param b The source code buffer.
 * @param index The starting index.
 */
std::optional<std::string> cmp::parseAlphanumericToken(pre::SegmentedCleanSource *b, ulong index){
    std::optional<std::string> r;
    std::optional<char> c = b->str[index];
    if(!c.has_value() || !std::isalpha(*c) || c != '_') return std::nullopt;
    *r += *c;

    ulong i = index + 1;
    while(isAlphanumericChar(b->str[i])) {
        *r += *b->str[i];
        ++i;
    }
    return r->empty() ? std::nullopt : r;
}


bool cmp::isAlphanumericChar(std::optional<char> const &c) {
    return c.has_value() && (std::isalnum(*c) || c == '_');
}






/**
 * @brief Parses the symbolic token that starts at index <index> of the buffer <b>.
 *      The token stops at the first character that is not part of its pattern.
 *      [!$%&*+\-/:<=>?@^`|~\[\].]+                 //! Symbolic identifiers
 *      [!$%&*+\-/:<=>?@^`|~;\[\]\(\)\{\}.\,]+      //! All symbolic tokens [what this function checks, includes identifiers]
 * @param b The source code buffer.
 * @param index The starting index.
 */
std::optional<std::string> cmp::parseSymbolicToken(pre::SegmentedCleanSource *b, ulong index){
    std::optional<std::string> r;

    ulong i = index + 1;
    while(isSymbolicChar(b->str[i])) {
        *r += *b->str[i];
        ++i;
    }
    return r->empty() ? std::nullopt : r;
}


bool cmp::isSymbolicChar(std::optional<char> const &c) {
    return c.has_value() && (
        c == '!' || c == '$' || c == '%' || c == '&' || c == '*' || c == '+' || c == '-' || c == ':' || c == '<' ||
        c == '=' || c == '>' || c == '?' || c == '@' || c == '^' || c == '`' || c == '|' || c == '~' || c == ';' ||
        c == '[' || c == ']' || c == '(' || c == ')' || c == '{' || c == '}' || c == '.' || c == ','
    );
}