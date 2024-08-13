#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <tuple>

#include "preprocessor.hpp"
#include "include.hpp"
#include "ElmCoords.hpp"
#include "Utils/utils.hpp"



namespace pre {

    /**
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param options The options.
     * @return The contents of the source file as a StructuredSource.
     */
    StructuredSource loadSourceCode(Options& options) {
        StructuredSource output;
        loadSourceCode_loop(options.sourceFile, options, output);
        return output;
    }




    static inline bool isLct(std::string b, ulong index) {
        return b[index] == '\\' && b[index + 1] == '\n';
    }





    //TODO add element metadata
    /**
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param filePath The path to the source file.
     * @param options The options.
     * @return The contents of the source file as a StructuredSource.
     */
    void loadSourceCode_loop(std::string filePath, Options& options, StructuredSource &output) { //TODO use options or remove the parameter if not needed
        std::string s = utils::readAndCheckFile(filePath);
        ulong curLine = 0; //The current line in the original file.

        StructuredSource r;
        ulong varLen = 0;               // The length of the current variable-size element (code or directive)
        ulong varLenHeight = 1;         // The height of the current variable-size element (code or directive)
        bool isVarDirective = false;    // False if the current variable-size element is arbitrary code, true if it is a directive (they can't both be on the same line)

        for(ulong i = 0; i < s.length();){
            ParsingResult pr;



            // Skip whitespace
            std::pair<ulong, ulong> wsRes = countWhitespaceCharacters(s, i, curLine, filePath);
            i += wsRes.first;
            curLine += wsRes.second;

            // Check for known elements and save the result
            if((pr = parseCharLiteral(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
            if((pr =  parseStrLiteral(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
            ;

            //FIXME parse default modules <> (or check it later, as it becomes part of the #directive element)




            // If the current index is inside a variable-length element
            if(pr.elmType == SourceElmType::NONE) {

                // If the current variable-length element contains a # character and is not a directive
                if(s[i] == '#' && !isVarDirective) {
                    // Push preceding characters as arbitrary code if there are any
                    if(varLen > 0) {
                        std::string elmTrueValue = s.substr(i - varLen, varLen);
                        r.elms.push_back(SourceElm(
                            SourceElmType::CODE,
                            elmTrueValue,
                            SourceElmMeta(i - varLen, filePath, varLenHeight, elmTrueValue)
                        ));
                    }

                    isVarDirective = true;  // Set the directive state to true
                    varLenHeight = 1;       // Reset the variable-length element height
                    varLen = 1;             // Reset and increase the variable-length element width
                    ++i;                    // Skip this character
                    continue;
                }

                // If a line continuation character is found
                else if(isLct(s, i)) {
                    ++curLine;       // Update the line counter
                    ++varLenHeight;  // Increase the variable-length element height
                    varLen += 2;     // Increase the variable-length element width
                    i += 2;          // Skip line continuation token token
                    continue;
                }

                // If a normal character is found
                if(s[i] == '\n') ++varLenHeight;
                ++varLen;  // Increase the variable-length element value
                ++i;       // Skip it
            }

            // If not
            else {
                // Push previous variable-length element if present
                if(varLen > 0) {
                    // Execute additional parsing for directives
                    if(isVarDirective) {
                        //FIXME choose if to save everything or check right now and only save payload

                    }
                    // Push other elements normally
                    else {
                        std::string elmTrueValue = s.substr(i - varLen, varLen);
                        r.elms.push_back(SourceElm(
                            SourceElmType::CODE,
                            elmTrueValue,
                            SourceElmMeta(i - varLen, filePath, varLenHeight, elmTrueValue)
                        ));
                    }
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


        // Push last variable-length element if present
        if(varLen > 0) {
            std::string elmTrueValue = s.substr(s.length() - 1 - varLen, varLen);
            r.elms.push_back(SourceElm(
                isVarDirective ? SourceElmType::DIRECTIVE : SourceElmType::CODE,
                elmTrueValue,
                SourceElmMeta(s.length() - 1, filePath, curLine, elmTrueValue)
            ));
        }



        mergeSourceElements(output, r);
    }








    // Add elements to output array. If include directives are found, parse the files and include them recursively
    void mergeSourceElements(StructuredSource &output, StructuredSource &r) {
        auto &e = r.elms;
        for(ulong i = 0; i < e.size();) {

            // Replace include directives with the specified file
            // if(e[i].type == SourceElmType::DIRECTIVE) {

                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // if(i < e.size() - 1 && e[i +1].type == SourceElmType::STRING) {

                // }
                // else utils::printError(
                //     utils::ErrType::PREPROCESSOR,
                //     ElmCoords(e[i].meta),
                //     "Missing file path in include directive. A string literal was expected, but could not be found."
                // );
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE

            //     continue;
            // }

            // Append anything else
            output.elms.push_back(e[i]);
            ++i;
        }
    }








    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Calculates the length of the data attached to a preprocessor directive.
     *     This function does NOT check if the payload contains valid data, but it does check if it's of the correct type based on the given directive.
     * @param b The string buffer that contains the payload.
     * @param index The index at which the payload starts.
     * @param DEBUG_curLine The current line number in the original file at which the payload starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return
     */
    void parseDirectivePayload(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        //FIXME actually parse it

    }









    //TODO add include backtrace with line and column number
    //TODO don't allow multi-byte characters inside of char literals
    /**
     * @brief Counts the whitespace characters in the string <b>, from the character at index <index> until the first non-whitespace character.
     *      Comments are considered whitespace.
     *      Line continuation tokens are counted as 2 whitespace characters.
     * @param b The string buffer that contains the whitespace.
     * @param index The index at which the whitespace starts.
     * @param DEBUG_curLine The current line number in the original file at which the whitespace starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return A pair containing the number of whitespace characters found and the number of lines they occupy
     */
    std::pair<ulong, ulong> countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        ulong i = index, h = 1;
        while(true) {
            // LCTs
            if(isLct(b, index)) {
                i += 2;
                ++h;
            }

            // Normal whitespace
            else if(b[index] == ' ' || b[index] == '\t') {
                ++i;
            }

            // Newlines
            else if(b[index] == '\n') {
                ++i;
                ++h;
            }


            else {
                // Comments
                std::pair<ulong, ulong> commentRes = countCommentCharacters(b, i, DEBUG_curLine + h - 1, DEBUG_filePath);
                if(commentRes.first > 0) {
                    i += commentRes.first;
                    h += commentRes.second - 1;
                }

                // Stop at non-whitespace characters (includes \0)
                else {
                    return std::pair<ulong, ulong>(i, h);
                }
            }
        }
    }








    //TODO check if multi-byte unicode characters can screw with the comment detection
    /**
     * @brief Calculates the length of the comment that strarts at index <index> and ends at the first newline character or at the end of the file.
     * @param b The string buffer that contains the comment.
     * @param index The index at which the comment starts.
     * @param DEBUG_curLine The current line number in the original file at which the comment starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return A pair containing the length of the comment and the lines it occupies, including the length of the opening and closing character sequences (including \\n but excluding \0).
     *     If the buffer doesn't contain a comment that starts at index <index>, (0, 1) is returned.
     */
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        if(b[index] != '/') return std::pair<ulong, ulong>(0, 1);


        char last = b[index];
        char commType = '\0'; // '\0' if unknow, '/' if single line, '*' if multiline
        ulong i = index + 1, h = 1;
        while(true) {
            // Check line continuation token
            if(isLct(b, i)) {
                ++h;
                i += 2;
            }

            // Starting sequence
            else if(commType == '\0') {
                if(b[i] == '/' || b[i] == '*') {
                    commType = b[i];
                    continue;
                }
                else {  //! Starting sequence not found (this includes \n and \0 cases)
                    return std::pair<ulong, ulong>(0, 1);
                }
            }

            // Single character closing sequences (End of file or single line comments)
            else if(b[i] == '\0' || commType == '/' && b[i] == '\n') {
                if(b[i] == '\n') {
                    ++h;
                    ++i;
                }
                break;
            }

            // Double character closing sequences (Multi line comments)
            else if(commType == '*' && last == '*' && b[i] == '/') {
                ++i;
                break;
            }

            // Normal characters (part of the comment)
            else {
                if(b[i] == '\n') ++h;
                last = b[i];
                ++i;
            }
        }


        return std::pair<ulong, ulong>(i, h);
    }








    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Parses the string literal that strarts at index <index> and ends at the first non-escaped " character.
     * @param b The string buffer that contains the string literal.
     * @param index The index at which the string literal starts.
     * @param DEBUG_curLine The current line number in the original file at which the string literal starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return Informations about the string literal, including the two " characters
     *     If the buffer doesn't contain a string literal that starts at index <index>, a result with type NONE is returned.
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

            // Malformed strings
            else if(b[i] == '\0') {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                    "String literal is missing a closing '\"' character."
                );
                exit(1);
            }
            else if(b[i] == '\n') {
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

        r.elmType = SourceElmType::STRING;
        return r;
    }








    /**
     * @brief Parses the char literal that starts at index <index> and checks if it contains a single 8-bit character (or a valid escape sequence).
     * @param b The string buffer that contains the char literal.
     * @param index The index at which the char literal starts.
     * @param DEBUG_curLine The current line number in the original file at which the char literal starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return Informations about the char literal, including the two ' characters.
     *     If the buffer doesn't contain a char literal that starts at index <index>, a result with type NONE is returned.
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

            // Closing sequence
            else if(last != '\\' && b[i] == '\'') {
                r.trueValue  += '\'';
                r.finalValue += '\'';
                break;
            }

            // Missing closing sequence
            else if(b[i] == '\0') {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                    "Char literal is missing a closing ' character."
                );
                exit(1);
            }
            else if(b[i] == '\n') {
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
            //FIXME check valid escape sequences
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

    //FIXME REMOVE COMMENTS BEFORE PROCESSING ANY OTHER PREPROCESSOR THING
}