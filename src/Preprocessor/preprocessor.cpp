#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <tuple>

#include "preprocessor.hpp"
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
        //FIXME LCTs CAN BE TERMINATED BY A \n.
        //FIXME CHECK THIS CASE. only skip 1 character if that happens
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
            curLine += wsRes.second - 1;

            // Check for known elements and save the result
            if((pr =   parseDirective(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
            if((pr = parseCharLiteral(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
            if((pr =  parseStrLiteral(s, i, curLine, filePath)).elmType == SourceElmType::NONE)
            ;

            //FIXME parse default modules <> (or check it later, as it becomes part of the #directive element)




            // If the current index is inside a variable-length element
            if(pr.elmType == SourceElmType::NONE) {

                // // If the current variable-length element contains a # character and is not a directive
                // //if(s[i] == '#' && !isVarDirective) {
                // if(s[i] == '#') {
                //     // Push preceding characters as arbitrary code if there are any
                //     if(varLen > 0) {
                //         std::string elmTrueValue = s.substr(i - varLen, varLen);
                //         r.elms.push_back(SourceElm(
                //             SourceElmType::CODE,
                //             elmTrueValue,
                //             SourceElmMeta(i - varLen, filePath, varLenHeight, elmTrueValue)
                //         ));
                //     }


                //     //isVarDirective = true;  // Set the directive state to true
                //     varLenHeight = 1;       // Reset the variable-length element height
                //     varLen = 1;             // Reset and increase the variable-length element width
                //     ++i;                    // Skip this character
                //     continue;
                // }

                // If a line continuation character is found

                //else if(isLct(s, i)) {
                if(isLct(s, i)) {
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
                    std::string elmTrueValue = s.substr(i - varLen, varLen);
                    r.elms.push_back(SourceElm(
                        SourceElmType::CODE,
                        elmTrueValue,
                        SourceElmMeta(i - varLen, filePath, varLenHeight, elmTrueValue)
                    ));
                }
                //isVarDirective = false;  // Set the directive state back to false


                // Push current parsed element
                r.elms.push_back(SourceElm(
                    pr.elmType,
                    pr.finalValue,
                    SourceElmMeta(i, filePath, curLine, pr.trueValue)
                ));
                curLine += pr.height - 1;  // Add the additional element height to the line counter
                i += pr.trueValue.length();


                varLen = 0;              // Set the variable-length element width back to 0
                varLenHeight = 0;        // Set the variable-length element height back to 0
            }
        }


        // Push last variable-length element if present
        if(varLen > 0) {
            std::string elmTrueValue = s.substr(s.length() - 1 - varLen, varLen);
            r.elms.push_back(SourceElm(
                SourceElmType::CODE,
                elmTrueValue,
                SourceElmMeta(s.length() - 1, filePath, curLine, elmTrueValue)
            ));
        }



        mergeSourceElements(output, r);
    }








    // Add elements to output array. If include statements are found, parse the files and include them recursively
    void mergeSourceElements(StructuredSource &output, StructuredSource &r) {
        auto &e = r.elms;
        for(ulong i = 0; i < e.size();) {

            // Replace include statements with the specified file
            // if(e[i].type == SourceElmType::DIRECTIVE) {

                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
                // if(i < e.size() - 1 && e[i +1].type == SourceElmType::STRING) {

                // }
                // else {
                //     utils::printError(
                //         utils::ErrType::PREPROCESSOR,
                //         ElmCoords(e[i].meta),
                //         "Missing file path in include statement. A string literal was expected, but could not be found."
                //     );
                //     exit(1);
                // }
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
            if(isLct(b, i)) {
                i += 2;
                ++h;
            }

            // Normal whitespace
            else if(b[i] == ' ' || b[i] == '\t') {
                ++i;
            }

            // Newlines
            else if(b[i] == '\n') {
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
                    return std::pair<ulong, ulong>(i - index, h);
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


        return std::pair<ulong, ulong>(i - index, h);
    }
















    //TODO check if multi-byte unicode characters can screw with the string detection
    /**
     * @brief Parses the preprocessor directive that starts at index <index>.
     *     This function does NOT check if the attached data contains valid data, but it does check if it's of the correct type based on the detected directive.
     *     It also prints an error if the directive is not recognized.
     * @param b The string buffer that contains the directive.
     * @param index The index at which the directive starts.
     * @param DEBUG_curLine The current line number in the original file at which the directive starts.
     * @param DEBUG_filePath The path to the file the buffer string was read from.
     * @return
     */
    ParsingResult parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
        ParsingResult r;
        if(b[index] != '#') return r;
        r.trueValue  += '#';
        //r.finalValue += '#';  //! Copied from trueValue after parsing the name of the directive


        ulong i = index + 1;
        std::smatch matchRes;
        if(std::regex_search(std::string::const_iterator(b.begin() + i), std::string::const_iterator(b.end()), matchRes, std::regex(R"(^([a-zA-Z]|(\\\n))+)"))){

            // Find directive name
            r.trueValue += matchRes[0].str(); //FIXME line numbers don't match
            r.finalValue = std::regex_replace(r.trueValue, std::regex(R"(\\\n)"), "");
            r.height += (r.trueValue.length() - r.finalValue.length()) / 2;
            i += r.trueValue.length() - 1;  //! -1 to account for the '#' character


            // Check if it is valid and set the source element type
            if(r.finalValue == "#include") { //FIXME add glob patterns to include statements
                r.elmType = SourceElmType::DIRECTIVE_INCLUDE;
            }
            else if(r.finalValue == "#define") {
                r.elmType = SourceElmType::DIRECTIVE_DEFINE;
            }
            else {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i - 1),
                    "Unknown preprocessor directive \"" + r.finalValue + "\"."
                );
                exit(1);
            }


            // Check if whitespace is present and skip it
            std::pair<ulong, ulong> wsCheck = countWhitespaceCharacters(b, index, DEBUG_curLine + r.height - 1, DEBUG_filePath);
            if(wsCheck.first > 0) {
                r.trueValue += b.substr(i, wsCheck.first);
                r.finalValue += " ";
                r.height += wsCheck.second - 1;
                i += wsCheck.first;
            }
            else {
                utils::printError(
                    utils::ErrType::PREPROCESSOR,
                    ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                    "Missing whitespace after include statement.\nThe name of the directive and its parameters must be separated by one or more whitespace characters."
                );
                exit(1);
            }


            // Parse string if the directive is an include statement
            if(r.elmType == SourceElmType::DIRECTIVE_INCLUDE && b[i]) {
                ParsingResult stringCheck = parseStrLiteral(b, i, DEBUG_curLine + r.height - 1, DEBUG_filePath);
                if(stringCheck.elmType == SourceElmType::STRING) {
                    r.finalValue += stringCheck.finalValue;
                    r.trueValue += stringCheck.trueValue;
                    r.height += stringCheck.height - 1;
                    i += stringCheck.trueValue.length();
                    //FIXME add standard module inclusion <>
                }
                else {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
                        "Missing file path in include statement. A string literal was expected, but could not be found."
                    );
                    exit(1);
                }
            }


            // Parse whole line if the directive is a define directive
            else if(r.elmType == SourceElmType::DIRECTIVE_DEFINE) {
                while(true) {
                    // Check line continuation token
                    if(isLct(b, i)) {
                        r.trueValue += "\\\n";
                        ++r.height;
                        i += 2;
                    }

                    // Check newline (terminates macro definitions)
                    else if(b[i] == '\n') {
                        r.finalValue += '\n';
                        r.trueValue += '\n';
                        ++r.height;
                        break;
                    }

                    // Check EOF (terminates macro definitions)
                    else if(b[i] == '\0') {
                        break;
                    }

                    // Normal characters (macro definition)
                    else {
                        r.finalValue += b[i];
                        r.trueValue += b[i];
                        ++i;
                    }
                }
            }

            // // Normal characters (part of the string)
            // else {
            //     r.trueValue  += b[i];
            //     r.finalValue += b[i];
            //     last = b[i];
            //     ++i;
            // }
        }


        return r;
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
                    "If you wish to use a newline character in the string, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
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
                    "If you wish to use a newline character in the char literal, use the escape sequence \"" + ansi::bold_cyan + "\\n" + ansi::reset + "\"."
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
                    "Did you mean " + ansi::bold_cyan + "'\\'" + ansi::reset + "?"
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