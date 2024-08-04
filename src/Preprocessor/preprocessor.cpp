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
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param options The options
     * @return The contents of the source file as a StructuredSource
     */
    StructuredSource loadSourceCode(Options& options) {
        return parseUnprocessedCode(options.sourceFile, options); //TODO include the files
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

        StructuredSource r;
        ulong varLen = 0;               // The length of the current variable-size element (code or directive)
        bool isVarDirective = false;    // False if the current variable-size element is arbitrary code, true if it is a directive (they can't both be on the same line)

        for(ulong i = 0; i < s.length();){
            //ulong len = 0;
            //SourceElmType elmType;
            ParsingResult pr;




            // Check for known elements and save the result
            if((pr = parseCharLiteral(s, i)).elmType == SourceElmType::NONE)
            if((pr =  parseStrLiteral(s, i)).elmType == SourceElmType::NONE)
            ;  (pr =     parseComment(s, i));




            // If the current index is inside a variable-length element
            if(pr.elmType == SourceElmType::NONE) {
                if(s[i] == '#') isVarDirective = true;  // Set the directive state to true if a '#' character is found in the wild
                ++varLen;                               // Increase the variable length value
                ++i;                                    // Skip this character
            }
            else {
                // Push previous variable-length element is present
                if(varLen > 0) {
                    std::string elmTrueValue = s.substr(i - varLen, varLen);
                    r.elms.push_back(SourceElm{
                        isVarDirective ? SourceElmType::DIRECTIVE : SourceElmType::CODE,
                        elmTrueValue,
                        SourceElmMeta{ i - varLen, "test file TODO", 0, elmTrueValue } //TODO save file & line number in meta
                    });
                }
                varLen = 0;                 // Set the variable length value back to 0
                isVarDirective = false;     // Set the directive state back to false


                // Push current parsed element
                r.elms.push_back(SourceElm{
                    pr.elmType,
                    pr.finalValue,
                    SourceElmMeta{ i, "test file TODO", 0, pr.trueValue} //TODO save file & line number in meta
                });
                i += pr.trueValue.length();
            }






/*
            ;    if(len = parseCharLiteral(s, i))     { elmType = SourceElmType::CHAR;    varLen = 0; }
            else if(len = parseStrLiteral(s, i)) { elmType = SourceElmType::STRING;  varLen = 0; }
            else if(len = parseComment(s, i))    { elmType = SourceElmType::COMMENT; varLen = 0; }
            else {
                ++varLen;
                ++i;
            }
            r.elms.push_back(SourceElm{ elmType, s.substr(i, len), SourceElmMeta{ i, "test file TODO" }});
            i += len;
*/
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
    ParsingResult parseComment(std::string b, ulong index) {
        ParsingResult r;
        if(b[index] != '/' || (b[index + 1] != '/' && b[index + 1] != '*')) return r;
        r.trueValue += '/';  r.trueValue += b[index + 1];
        r.finalValue += '/'; r.finalValue += b[index + 1];


        ulong i = index + 2;
        while(true) {
            // Check line continuation token
            if(b[i] == '\\' && b[i + 1] == '\n') {
                r.trueValue += "\\\n";
                i += 2;
            }
            // Check closing sequences
            else {
                // Single character closing sequences (End of file or single line comments)
                if(b[i] == '\0' || (b[index + 1] == '/' && b[i] == '\n')) {
                    r.trueValue += b[i];
                    r.finalValue += b[i];
                    break;
                }

                // Double character closing sequences (Multi line comments)
                else if(b[index + 1] == '*' && b[i] == '*' && b[i + 1] == '/') {
                    r.trueValue += "*/";
                    r.finalValue += "*/";
                    break;
                }

                // Normal characters (part of the comment)
                else {
                    r.trueValue += b[i];
                    r.finalValue += b[i];
                    ++i;
                }
            }
        }

        r.elmType = SourceElmType::COMMENT;
        return r;
        //return i - index + 1;
    }





    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Calculates the length of the string literal that strarts at index <index> and ends at the first non-escaped " character.
     * @param b The string buffer that contains the string literal.
     * @param index The index at which the string literal starts.
     * @return The length of the string literal, including the two " characters
     *     If the buffer doesn't contain a string literal that starts at index <index>, 0 is returned.
     */
    ParsingResult parseStrLiteral(std::string b, ulong index) {
        ParsingResult r;
        if(b[index] != '"') return r;
        r.trueValue += '"';
        r.finalValue += '"';


        ulong i = index + 1;
        while(true) {
            // Check line continuation token
            if(b[i] == '\\' && b[i + 1] == '\n') {
                r.trueValue += "\\\n";
                i += 2;
            }
            // Check closing sequences
            else {
                // Malformed strings
                if(b[i] == '\0' || b[i] == '\n') {
                    utils::printError(utils::ErrType::PREPROCESSOR, "String literal is missing a closing '\"' character.");
                    exit(1);
                }

                // Closing sequence
                else if(b[i] == '"' && i > 0 && b[i - 1] != '\\') {
                    r.trueValue += '"';
                    r.finalValue += '"';
                    break;
                }

                // Normal characters (part of the string)
                else {
                    r.trueValue += b[i];
                    r.finalValue += b[i];
                    ++i;
                }
            }
        }

        r.elmType = SourceElmType::STRING;
        return r;
        //return i - index + 1;
    }



    /**
     * @brief Checks that the char literal that starts at index <index> contains a single 8-bit character.
     * @param b The string buffer that contains the char literal.
     * @param index The index at which the char literal starts.
     * @return The value 3.
     *     If the buffer doesn't contain a char literal that starts at index <index>, no error is printed and 0 is returned.
     */
    ParsingResult parseCharLiteral(std::string b, ulong index) {
        ParsingResult r;
        if(b[index] != '\'') return r;
        r.trueValue += '\'';
        r.finalValue += '\'';


        if(b[index + 1] == '\'') {
            utils::printError(utils::ErrType::PREPROCESSOR, "Char literal cannot be empty.");
            exit(1);
        }
        //TODO check if the actual character is valid. \n \0 and others are not valid. they need to be escaped
        //TODO Print a special error if they are actually used raw. tell the user to escape them
        //TODO

        //FIXME use a loop to detect and save the whole malformed literal
        if(b[index + 2] == '\0' || b[index + 2] == '\n') {
            utils::printError(utils::ErrType::PREPROCESSOR, "Char literal is missing a closing ' character.");
            exit(1);
        }
        if(b[index + 2] != '\'') {
            utils::printError(
                utils::ErrType::PREPROCESSOR,
                "Char literal contains more than one byte. This is not allowed.\n"
                "If you wish to store strings or a multi-bye Unicode character, you can use the \"str\" type (module <string>)." //TODO check if this is the correct type
            );
            exit(1);
        }

        r.elmType = SourceElmType::CHAR;
        return r;
    }

    //TODO REMOVE COMMENTS BEFORE PROCESSING ANY OTHER PREPROCESSOR THING
    //TODO CHECK ESCAPED CHARACTER. they are valid char literals.
}