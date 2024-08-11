#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>

#include "preprocessor.hpp"
#include "include.hpp"
#include "ElmCoords.hpp"
#include "Utils/utils.hpp"



namespace pre {

    /**
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param options The options
     * @return The contents of the source file as a StructuredSource
     */
    StructuredSource loadSourceCode(Options& options) {
        return parseUnprocessedCode(options.sourceFile, options); //TODO include the files
    }




    static inline bool isLct(std::string b, ulong index) {
        return b[index] == '\\' && b[index + 1] == '\n';
    }





    //TODO add element metadata
    /**
     * @brief Loads a source file using its path and the provided options and converts it to a StructuredSource
     * @param filePath The path to the source file
     * @param options The options
     * @return The generated StructuredSource value
     */
    StructuredSource parseUnprocessedCode(std::string filePath, Options& options) { //TODO use options or remove the parameter if not needed
        std::string s = utils::readAndCheckFile(filePath);
        ulong curLine = 0; //The current line in the original file.

        StructuredSource r;
        ulong varLen = 0;               // The length of the current variable-size element (code or directive)
        ulong varLenHeight = 1;         // The height of the current variable-size element (code or directive)
        bool isVarDirective = false;    // False if the current variable-size element is arbitrary code, true if it is a directive (they can't both be on the same line)

        for(ulong i = 0; i < s.length();){
            ParsingResult pr;




            // Check for known elements and save the result
            if((pr = parseCharLiteral(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
            if((pr =  parseStrLiteral(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
               (pr =     parseComment(s, i, curLine, filePath));
            //FIXME parse default modules <> (or check it later, as it becomes part of the #directive element)




            // If the current index is inside a variable-length element
            if(pr.elmType == SourceElmType::NONE) {

                // If the current variable-length element contains a # character
                if(s[i] == '#') {
                    isVarDirective = true;  // Set the directive state to true
                    ++varLen;               // Increase the variable length value
                    ++i;                    // Skip this character
                    continue;
                }

                // If a line continuation character is found
                else if(isLct(s, i)) {
                    ++curLine;       // Update the line counter
                    ++varLenHeight;  // Increase the variable-length element width
                    varLen += 2;     // Increase the variable-length element value
                    i += 2;          // Skip line continuation token token
                    continue;
                }

                // If a normal character is found
                if(s[i] == '\n') ++varLenHeight;
                ++varLen;  // Increase the variable-length element value
                ++i;       // Skip it
            }
            else {
                // Push previous variable-length element is present
                if(varLen > 0) {
                    std::string elmTrueValue = s.substr(i - varLen, varLen);
                    r.elms.push_back(SourceElm(
                        isVarDirective ? SourceElmType::DIRECTIVE : SourceElmType::CODE,
                        elmTrueValue,
                        SourceElmMeta(i - varLen, filePath, varLenHeight, elmTrueValue)
                    ));
                }
                varLen = 0;              // Set the variable-length element width back to 0
                varLenHeight = 0;        // Set the variable-length element height back to 0
                isVarDirective = false;  // Set the directive state back to false


                // Push current parsed element
                r.elms.push_back(SourceElm(
                    pr.elmType,
                    pr.finalValue,
                    SourceElmMeta(i, filePath, curLine, pr.trueValue)
                ));
                curLine += pr.height - 1;  // Add the additional element height to the line counter
                i += pr.trueValue.length();
            }
        }

        return r;
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
    //TODO don't allow multi-byte characters inside of char literals







    //TODO check if multi-byte unicode characters can screw with the comment detection
    /**
     * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
     * @param b The string buffer that contains the comment.
     * @param index The index at which the comment starts.
     * @param DEBUG_curLine The current line number in the original file at which the comment starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return The length of the comment, including the length of the opening and closing character sequences.
     *     If the buffer doesn't contain a comment that starts at index <index>, 0 is returned.
     */
    ParsingResult parseComment(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        ParsingResult r;
        if(b[index] != '/') return r;
        r.trueValue  += '/';
        r.finalValue += '/';


        char last = b[index];
        char commType = '\0'; // '\0' if unknow, '/' if single line, '*' if multiline
        ulong i = index + 2;
        while(true) {
            // Check line continuation token
            if(isLct(b, i)) {
                r.trueValue += "\\\n";
                ++r.height;
                i += 2;
            }
            else {
                // Starting sequence
                if(commType == '\0') {
                    if(b[i] == '/' || b[i] == '*') {
                        r.trueValue  += b[i];
                        r.finalValue += b[i];
                        commType = b[i];
                        continue;
                    }
                    else {  //! Starting sequence not found (this includes \n and \0 cases)
                        return r;
                    }
                }

                // Single character closing sequences (End of file or single line comments)
                else if(b[i] == '\0' || commType == '/' && b[i] == '\n') {
                    if(b[i] == '\n') ++r.height;
                    r.trueValue  += b[i];
                    r.finalValue += b[i];
                    break;
                }

                // Double character closing sequences (Multi line comments)
                else if(commType == '*' && last == '*' && b[i] == '/') {
                    r.trueValue  += '/';
                    r.finalValue += '/';
                    break;
                }

                // Normal characters (part of the comment)
                else {
                    if(b[i] == '\n') ++r.height;
                    last = b[i];
                    r.trueValue  += b[i];
                    r.finalValue += b[i];
                    ++i;
                }
            }
        }

        r.elmType = SourceElmType::COMMENT;
        return r;
    }








    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Calculates the length of the string literal that strarts at index <index> and ends at the first non-escaped " character.
     * @param b The string buffer that contains the string literal.
     * @param index The index at which the string literal starts.
     * @param DEBUG_curLine The current line number in the original file at which the string literal starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return The length of the string literal, including the two " characters
     *     If the buffer doesn't contain a string literal that starts at index <index>, 0 is returned.
     */
    ParsingResult parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        ParsingResult r;
        if(b[index] != '"') return r;
        r.trueValue  += '"';
        r.finalValue += '"';


        char last = b[index];
        ulong i = index + 1;
        while(true) {
            // Check line continuation token
            if(isLct(b, i)) {
                r.trueValue += "\\\n";
                ++r.height;
                i += 2;
            }
            else {
                // Malformed strings
                if(b[i] == '\0') {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                        "String literal is missing a closing '\"' character."
                    );
                    exit(1);
                }
                if(b[i] == '\n') {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                        "String literal is missing a closing '\"' character.\n"
                        "If you wish to use a newline character in the string, use the escape sequence \"" + ansi::fgCyan + ansi::bold + "\\n" + ansi::reset + "\"."
                    );
                    exit(1);
                }

                // Closing sequence
                else if(last != '\\' && b[i] == '"') {
                    r.trueValue  += '"';
                    r.finalValue += '"';
                    break;
                }

                // Normal characters (part of the string)
                else {
                    r.trueValue  += b[i];
                    r.finalValue += b[i];
                    last = b[i];
                    ++i;
                }
            }
        }

        r.elmType = SourceElmType::STRING;
        return r;
    }








    /**
     * @brief Checks that the char literal that starts at index <index> contains a single 8-bit character.
     * @param b The string buffer that contains the char literal.
     * @param index The index at which the char literal starts.
     * @param DEBUG_curLine The current line number in the original file at which the char literal starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return The value 3.
     *     If the buffer doesn't contain a char literal that starts at index <index>, no error is printed and 0 is returned.
     */
    ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        ParsingResult r;
        if(b[index] != '\'') return r;
        r.trueValue  += '\'';
        r.finalValue += '\'';


        // Check for empty char literals or unescaped ' characters

        //TODO check if the actual character is valid. \n \0 and others are not valid. they need to be escaped
        //TODO Print a special error if they are actually used raw. tell the user to escape them
        //TODO



        char last = b[index];
        ulong i = index + 1;
        while(true) {
            // Check line continuation token
            if(isLct(b, i)) {
                r.trueValue += "\\\n";
                ++r.height;
                i += 2;
            }
            else {
                // Closing sequence
                if(last != '\\' && b[i] == '\'') {
                    r.trueValue  += '\'';
                    r.finalValue += '\'';
                    break;
                }

                // Missing closing sequence
                if(b[i] == '\0') {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                        "Char literal is missing a closing ' character."
                    );
                    exit(1);
                }
                if(b[i] == '\n') {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                        "Char literal is missing a closing ' character.\n"
                        "If you wish to use a newline character in the char literal, use the escape sequence \"" + ansi::fgCyan + ansi::bold + "\\n" + ansi::reset + "\"."
                    );
                    exit(1);
                }

                // Normal characters and escape sequences
                else {
                    r.trueValue  += b[i];
                    r.finalValue += b[i];
                    last = b[i];
                    ++i;
                }
            }
        }




        ulong finalLen = r.finalValue.length();
        if(finalLen > 4 || finalLen == 4 && r.finalValue[1] != '\\') {
            utils::printError(
                utils::ErrType::PREPROCESSOR,
                ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                "Char literal contains more than one byte. This is not allowed.\n"
                "If you wish to store strings or a multi-bye Unicode character, you can use the str type (module <string>)." //TODO check if this is the correct type
            );
            exit(1);
        }
        else if(r.finalValue.length() == 4) { //!  && r.finalValue[1] == '\\'
            //TODO check valid escape sequences
        }
        else if(r.finalValue.length() == 3) {
            if(r.finalValue[1] == '\\') {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                    "Char literal is missing a closing ' character.\n"
                    "Did you mean " + ansi::fgCyan + ansi::bold + "'\\'" + ansi::reset + "?"
                );
                exit(1);
            }
            //! Else return;
        }
        else if(r.finalValue.length() == 2) {
            utils::printError(
                utils::ErrType::PREPROCESSOR,
                ElmCoords(DEBUG_filePath, DEBUG_curLine, index, i),
                "Char literal cannot be empty."
            );
            exit(1);
        }



        r.elmType = SourceElmType::CHAR;
        return r;
    }

    //TODO REMOVE COMMENTS BEFORE PROCESSING ANY OTHER PREPROCESSOR THING
}