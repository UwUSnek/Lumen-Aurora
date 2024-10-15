#include <sstream>
#include "keywordsIdentifiers.hpp"








/**
 * @brief Parses the alphanumeric token that starts at index <index> of the buffer <b>.
 *      The token stops at the first character that is not part of its pattern.
 *      [a-zA-Z_][a-zA-Z_0-9]*      //! Alphanumeric identifiers
 *      [a-zA-Z_][a-zA-Z_0-9]*      //! All alphanumeric tokens [what this function checks, includes alphanumeric identifiers]
 * @param b The source code buffer.
 * @param index The starting index.
 */
std::optional<std::string> cmp::parseAlphanumericToken(pre::SegmentedCleanSource *b, ulong index){
    std::stringstream r;
    std::optional<char> c = b->str[index];
    if(!isAlphanumericCharFirst(*c)) return std::nullopt;
    r << *c;

    ulong i = index + 1;
    while(isAlphanumericChar(b->str[i])) {
        r << *b->str[i];
        ++i;
    }
    return r.tellp() == 0 ? std::nullopt : (std::optional<std::string>)r.str();
}




/**
 * @brief Checks if a character can be the first character of an alphanumeric identifier, a meta keyword or a keyword.
 * @param c The character to check.
 * @return Wether the character can be the first character of these elements.
 */
bool cmp::isAlphanumericCharFirst(std::optional<char> const &c) {
    return c.has_value() && (std::isalpha(*c) || c == '_');
}




/**
 * @brief Checks if a character can be part of an alphanumeric identifier, a meta keyword or a keyword.
 * @param c The character to check.
 * @return Wether the character can be part of these elements.
 */
bool cmp::isAlphanumericChar(std::optional<char> const &c) {
    return c.has_value() && (std::isalnum(*c) || c == '_');
}








/**
 * @brief Parses the symbolic token that starts at index <index> of the buffer <b>.
 *      The token stops at the first character that is not part of its pattern.
 *      [!$%&*+\-/:<=>?@^`|~\[\].]+                 //! Symbolic identifiers
 *      [!$%&*+\-/:<=>?@^`|~\[\].,;\(\)\{\}]+       //! All symbolic tokens [what this function checks, includes identifiers]
 * @param b The source code buffer.
 * @param index The starting index.
 */
std::optional<std::string> cmp::parseSymbolicToken(pre::SegmentedCleanSource *b, ulong index){
    std::stringstream r;
    if(isCharReserved(*b->str[index])) {
        return std::string(1, *b->str[index]);
    }


    ulong i = index;
    while(isSymbolicChar(b->str[i])) {
        r << *b->str[i];
        ++i;
    }
    return r.tellp() == 0 ? std::nullopt : (std::optional<std::string>)r.str();
}




/**
 * @brief Checks if a character can be part of either a symbolic identifier, a meta keyword or a keyword.
 * @param c The character to check.
 * @return Wether the character can be part of these elements.
 */
bool cmp::isSymbolicChar(std::optional<char> const &c) {
    return c.has_value() && (
        c == '!' || c == '%' || c == '&' || c == '*' || c == '+' || c == '-' || c == ':' || c == '<' || c == '[' ||
        c == '='             || c == '?' || c == '^' || c == '`' || c == '|' || c == '~' || c == '>' || c == ']'
    );
}




/**
 * @brief Checks if a character is reserved for keywords.
 *      ! This kind of keywords can only be composed by a single character as
 *      ! they are allowed to be near each other without the need of whitespace between them. //FIXME specify this in the documentation. maybe put them in a different category. Strong keywords or something
 * @param c The character to check.
 * @return Wether the character is a keyword.
 */
bool cmp::isCharReserved(std::optional<char> const &c) {
    return c.has_value() && (
        c == '$' || c == '@' ||
        c == ';' || c == ',' ||
        c == '(' || c == ')' ||
        c == '{' || c == '}' ||
        c == '.'
    );
}