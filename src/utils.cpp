#include <string>
#include <iostream>
#include <regex>

#include "utils.hpp"






namespace utils {
    /**
     * @brief Prints an error to stderr, specifying the error type.
     *      This function doesn't stop the program.
     * @param errorType The type of the error
     * @param message The error message. This can contain multiple lines.
     *      One \n character is automatically added at the end of the string.
     */
    void printError(ErrType errorType, std::string message) {
        std::cerr << ansi::fgRed << ansi::bold;
        switch(errorType) {
            case ErrType::COMMAND:      { std::cerr << "Could not parse terminal command:"; break; }
            case ErrType::PREPROCESSOR: { std::cerr << "Preprocessor error:";               break; }
            case ErrType::COMPILER:     { std::cerr << "Compilation error:";                break; }
        }
        std::cerr << ansi::reset << "\n";

        std::cerr << "    " << std::regex_replace(message, std::regex("\n"), "\n    ") << "\n";
    }




    //TODO add include backtrace with line and column number
    //TODO add line display in error message

    //TODO don't allow multi-byte characters inside of char literals

    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Calculates the length of the string literal that strarts at index <index> and ends at the first non-escaped " character.
     * @param buffer The string buffer that contains the string literal
     * @param index The index at which the string literal starts.
     *      This function assumes that buffer[i] contains the character " (double quotation mark)
     * @return The length of the string literal, including the two " characters
     */
    ulong computeStrLiteralLen(std::string buffer, ulong index) {
        ulong i = index + 1;
        for(; buffer[i] != '\0'; ++i) {
            char c = buffer[i];
            if(c == '\0' || c == '\n') {
                printError(ErrType::PREPROCESSOR, "String literal is missing a closing '\"' character.");
                exit(1);
            }
            if(c == '"' && buffer[i - 1] != '\\') break;
        }
        return i - index + 1;
    }



    /**
     * @brief Checks that the char literal that starts at index <index> contains a single 8-bit character
     * @param buffer The string buffer that contains the char literal
     * @param index The index at which the char literal starts.
     *      This function assumes that buffer[i] contains the character ' (single quotation mark)
     */
    void checkCharLiteral(std::string buffer, ulong index) {
        if(buffer[index + 1] == '\'') {
            printError(ErrType::PREPROCESSOR, "Char literal cannot be empty.");
            exit(1);
        }
        //TODO check if the actual character is valid. \n \0 and others are not valid. they need to be escaped
        //TODO Print a special error if they are actually used raw. tell the user to escape them
        //TODO
        if(buffer[index + 2] == '\0' || buffer[index + 2] == '\n') {
            printError(ErrType::PREPROCESSOR, "Char literal is missing a closing ' character.");
            exit(1);
        }
        if(buffer[index + 2] != '\'') {
            printError(
                ErrType::PREPROCESSOR,
                "Char literal contains more than one byte. This is not allowed.\n"
                "If you wish to store strings or multi-bye Unicode character, you should use the \"str\" type (module <string>)." //TODO check if this is the correct type
                );
            exit(1);
        }
    }

    //TODO REMOVE COMMENTS BEFORE PROCESSING ANY OTHER PREPROCESSOR THING
    //TODO CHECK ESCAPED CHARACTER. they are valid char literals.
}


