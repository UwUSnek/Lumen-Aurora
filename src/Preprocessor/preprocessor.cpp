#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>

#include "preprocessor.hpp"
#include "include.hpp"
#include "utils.hpp"



namespace pre {
    /**
     * @brief Reads a single source file and returns its contents as a string.
     *     Line continuation tokens are automatically recognized and replaced.
     *     Prints an error message if the file cannot be opened or doesn't exist.
     * @param fileName The path to the file
     * @return The contents of the file
     */
    static std::string readAndCheckFile(std::string fileName) {
        // Create file stream and print an error if the file cannot be opened
        std::ifstream f(fileName);
        if(!f) utils::printError(
            utils::ErrType::PREPROCESSOR,
            "Could not open file \"" + fileName + "\": " + std::strerror(errno) + ".\n" +
            "Current working directory is: " + std::string(std::filesystem::current_path()) + "."
        );

        // Read all the lines and return a string containing all of them
        std::string l, r;
        while(getline(f, l)) {
            r += l;
            // Remove line continuation tokens and merge lines if needed
            if(l[l.length() - 1] != '\\') r += "\n"; else r.resize(r.length() - 1);
        }
        return r;
    }




    /**
     * @brief Loads the source file using the provided options without processing any of its directives.
     *     Line continuation tokens are automatically recognized and replaced
     * @param options The options
     * @return The contents of the source file as a string
     */
    std::string loadSourceFile(Options& options) {
        return readAndCheckFile(options.sourceFile);
    }



    //TODO add element metadata
    /**
     * @brief Reads a source file and converts it to a StructuredSource
     * @param s The source file as a string
     * @return The generated StructuredSource value
     */
    StructuredSource parseSourceFile(std::string s) { //FIXME call this function from somewhere. prob straight from processDirectives, before actually stitching the files and expanding the macros
        StructuredSource r;

        for(ulong i = 0; i < s.length();){
            ulong len = 0;
            SourceElmType elmType;
            ;    if(len = checkCharLiteral(s, i))     elmType = SourceElmType::CHAR;
            else if(len = computeStrLiteralLen(s, i)) elmType = SourceElmType::STRING;
            else if(len = computeCommentLen(s, i))    elmType = SourceElmType::COMMENT;
            else {
                ++i;
                //TODO arbitrary code & directives
            }
            r.elms.push_back(SourceElm{ elmType, s.substr(i, len), SourceElmMeta{ i, "test file TODO" }});
            i += len;
        }

        return r;
    }




    /**
     * @brief Finds and processes preprocessor directives.
     * @param s The source file as a string
     * @return The full source code as a string, without any preprocessor directives
     */
    StructuredSource processDirectives(std::string s) {
        //std::string r;
        //r = pre::stitchFiles(s);
        return parseSourceFile(s);
    }




//     /**
//      * @brief Converts char and string literals to their numerical representation. //TODO
//      * @param sourceFile The source file as a string
//      * @return The modified source file as a string
//      */
//     std::string convertStrings(std::string sourceFile) {
//         std::string r;
//         for()
//         //if(sourceFile[i] == '"' && (i == 0 || sourceFile[i - 1])) {
//     }

















    //TODO add include backtrace with line and column number
    //TODO add line display in error message

    //TODO don't allow multi-byte characters inside of char literals





    //TODO check if multi-byte unicode characters can screw with the comment detection
    /**
     * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
     * @param b The string buffer that contains the comment.
     * @param index The index at which the comment starts.
     * @return The length of the comment, including the length of the opening and closing character sequences.
     *     If the buffer doesn't contain a comment that starts at index <index>, 0 is returned.
     */
    ulong computeCommentLen(std::string b, ulong index) {
        if(b[index] != '/' || (b[index + 1] != '/' && b[index + 1] != '*')) return 0;

        ulong i = index + 2;
        for(;; ++i) {
            if(b[i] == '\0' || (b[index + 1] == '/' && b[i] == '\n')) break;     //! Single character closing sequences (End of file or single line comments)
            if(b[index + 1] == '*' && b[i] == '*' && b[i + 1] == '/') {          //! Double character closing sequences (Multi line comments)
                ++i;  // Skip first character of the closing sequence
                break;
            }
        }
        return i - index + 1;
    }





    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Calculates the length of the string literal that strarts at index <index> and ends at the first non-escaped " character.
     * @param b The string buffer that contains the string literal.
     * @param index The index at which the string literal starts.
     * @return The length of the string literal, including the two " characters
     *     If the buffer doesn't contain a string literal that starts at index <index>, 0 is returned.
     */
    ulong computeStrLiteralLen(std::string b, ulong index) {
        if(b[index] != '"') return 0;

        ulong i = index + 1;
        for(;; ++i) {
            if(b[i] == '\0' || b[i] == '\n') {
                utils::printError(utils::ErrType::PREPROCESSOR, "String literal is missing a closing '\"' character.");
                exit(1);
            }
            if(b[i] == '"' && b[i - 1] != '\\') break;
        }
        return i - index + 1;
    }



    /**
     * @brief Checks that the char literal that starts at index <index> contains a single 8-bit character.
     * @param b The string buffer that contains the char literal.
     * @param index The index at which the char literal starts.
     * @return The value 3.
     *     If the buffer doesn't contain a char literal that starts at index <index>, no error is printed and 0 is returned.
     */
    ulong checkCharLiteral(std::string b, ulong index) {
        if(b[index] != '\'') return 0;

        if(b[index + 1] == '\'') {
            utils::printError(utils::ErrType::PREPROCESSOR, "Char literal cannot be empty.");
            exit(1);
        }
        //TODO check if the actual character is valid. \n \0 and others are not valid. they need to be escaped
        //TODO Print a special error if they are actually used raw. tell the user to escape them
        //TODO
        if(b[index + 2] == '\0' || b[index + 2] == '\n') {
            utils::printError(utils::ErrType::PREPROCESSOR, "Char literal is missing a closing ' character.");
            exit(1);
        }
        if(b[index + 2] != '\'') {
            utils::printError(
                utils::ErrType::PREPROCESSOR,
                "Char literal contains more than one byte. This is not allowed.\n"
                "If you wish to store strings or multi-bye Unicode character, you should use the \"str\" type (module <string>)." //TODO check if this is the correct type
                );
            exit(1);
        }

        return 3;
    }

    //TODO REMOVE COMMENTS BEFORE PROCESSING ANY OTHER PREPROCESSOR THING
    //TODO CHECK ESCAPED CHARACTER. they are valid char literals.
}