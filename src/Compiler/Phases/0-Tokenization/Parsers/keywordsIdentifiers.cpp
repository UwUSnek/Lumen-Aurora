#include <sstream>
#include "keywordsIdentifiers.hpp"








/**
 * @brief Parses the alphanumeric token that starts at index <index> of the buffer <b>.
 *      The token stops before the first character that is not part of its pattern.
 *      [a-zA-Z_][a-zA-Z_0-9]*      //! Alphanumeric identifiers
 *      [a-zA-Z_][a-zA-Z_0-9]*      //! All alphanumeric tokens [what this function checks, includes alphanumeric identifiers]
 * @param b The source code buffer.
 * @param index The starting index.
 * @return The string value of the token, or an empty string if one was not found.
 */
std::string cmp::parseAlphanumericToken(ptr<pre::AnnotatedSource<false>> b, ulong index){

    // Check and store first character
    const auto c0 = (*b)[index];
    if(!c0 || !isAlphanumericCharFirst(*c0)) return "";
    std::stringstream r;
    r << *c0;

    // Check and store other characters
    ulong i = index + 1;
    while(true) {
        const auto c = (*b)[i];
        if(!c || !isAlphanumericChar(*c)) return r.str();
        r << *c;
        ++i;
    }
}




/**
 * @brief Checks if a character can be the first character of an alphanumeric identifier, a meta keyword or a keyword.
 * @param c The character to check.
 * @return Wether the character can be the first character of these elements.
 */
bool cmp::isAlphanumericCharFirst(const char c) {
    return std::isalpha(c) || c == '_';
}




/**
 * @brief Checks if a character can be part of an alphanumeric identifier, a meta keyword or a keyword.
 * @param c The character to check.
 * @return Wether the character can be part of these elements.
 */
bool cmp::isAlphanumericChar(const char c) {
    return std::isalnum(c) || c == '_';
}








/**
 * @brief Parses the symbolic identifier that starts at index <index> of the buffer <b>.
 *      The identifier stops before the first character that is not part of its pattern.
 *      The pattern of a symbolic identifier is: [!%&*+\-/:<=>?^`|~\[\]]+.
 * @param b The source code buffer.
 * @param index The starting index.
 * @return The string value of the identifier, or an empty string if one was not found.
 */
std::string cmp::parseSymbolicIdentifier(ptr<pre::AnnotatedSource<false>> b, ulong index){
    //! Creating an empty string every time this fails is prob faster than creating a stringstream.
    //! Short string concatenation overhead is negligible
    //! Checking the first character before allocating would be better, but that makes the code less maintainable.
    std::string r;
    ulong i = index;
    while(true) {
        const auto c = (*b)[i];
        if(!c || !isSymbolicChar(*c)) return r;
        r += *c;
        ++i;
    }
}




/**
 * @brief Parses the symbolic keyword that starts at index <index> of the buffer <b>.
 *      Symbolic keywords are at most 1 character long.
 *      The pattern of a symbolic keyword is: [@$,;\(\)\{\}.].
 * @param b The source code buffer.
 * @param index The starting index.
 * @return The string value of the keyword, or an empty string if one was not found.
 */
std::string cmp::parseSymbolicKeyword(ptr<pre::AnnotatedSource<false>> b, ulong index){
    const auto c = (*b)[index];
    if(!c || !isCharReserved(*c)) return "";
    return std::string(1, *c);
}




static bool __internal_isSymbolicChar_arr[128]{ false };
static int __internal_init_a = [](){
    for(char c : std::string("!$%&*+-/<=>?[]^`|~")) {
        __internal_isSymbolicChar_arr[(int)c] = true;
    }
    return 0;
}();
/**
 * @brief Checks if a character can be part of a symbolic identifier.
 * @param c The character to check.
 * @return Wether the character can be part of a symbolic identifier.
 */
bool cmp::isSymbolicChar(const char c) {
    return __internal_isSymbolicChar_arr[(int)c];
}






static bool __internal_isCharReserved_arr[128]{ false };
static int __internal_init_b = [](){
    for(char c : std::string(":;.,(){}@")) {
        __internal_isSymbolicChar_arr[(int)c] = true;
    }
    return 0;
}();
/**
 * @brief Checks if a character is reserved for keywords.
 *      ! This kind of keywords can only be composed by a single character as
 *      ! they are allowed to be near each other without the need of whitespace between them. //FIXME specify this in the documentation. maybe put them in a different category. Strong keywords or something
 * @param c The character to check.
 * @return Wether the character is a keyword.
 */
bool cmp::isCharReserved(const char c) {
    return __internal_isCharReserved_arr[(int)c];
}