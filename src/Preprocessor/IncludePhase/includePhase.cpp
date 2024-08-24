#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "Preprocessor/preprocessor.hpp"
#include "includePhase.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Preprocessor/CleanupPhase/cleanupPhase.hpp"






//FIXME fix line references
namespace pre {
    //TODO SPLIT FUNCTION INTO MANY, MORE READABLE FUNCTIONS
    //TODO SPLIT FUNCTION INTO MANY, MORE READABLE FUNCTIONS
    //TODO SPLIT FUNCTION INTO MANY, MORE READABLE FUNCTIONS
    //TODO SPLIT FUNCTION INTO MANY, MORE READABLE FUNCTIONS
    //TODO SPLIT FUNCTION INTO MANY, MORE READABLE FUNCTIONS
    SegmentedCleanSource startIncludePhase(SegmentedCleanSource &b, ulong DBG_filePathIndex) {
        SegmentedCleanSource r;




        ulong i = 0;                // The character index relative to the current file, not including included files
        ulong sgmi = 0;             // The character index relative to the original version of the current file, not including included files
        ulong OG_curLine = 0;       // The current line number relative to the original version of the current file, not including included files
        ulong OG_i = 0;
        while(i < b.str.length()) {

            // Push precending deleted elements
            while(sgmi < b.sgm.size() && b.sgm[sgmi].isRemoved) {      // For each preceding deleted element
                r.sgm.push_back(b.sgm[sgmi]);       // Push it
                OG_i += r.sgm[sgmi].len;            // Update original character index counter
                ++sgmi;                             // Increase segment counter
            }

            //FIXME check if line number in errors at line 0 is correct
            // If an include directive is detected, replace it with the contents of the file
            ulong includeLineNum = b.sgm[sgmi].OG_lineNum;
            for(ulong j = OG_i; j > b.sgm[sgmi].OG_start; --j) if(b.str[j] == '\n') ++includeLineNum;
            std::smatch match;
            if(std::regex_search(b.str.cbegin() + i, b.str.cend(), match, std::regex(R"(^#include[ \t]*)"))) {

                ElmCoords relevantCoords(sourceFilePaths[DBG_filePathIndex], includeLineNum, OG_i, OG_i + match[0].length()); //FIXME set line numberF  //FIXME FIX ELEMENT WIDTH
                i += match[0].length(); //FIXME ^ pass the index to ElmCoords instead of the string value
                // i2 += match[0].length();

                // Detect specified file path                                                 //     ╭────── file ──────╮ ╭───── module ─────╮
                std::smatch filePathMatch;                                                    //     │ ╭────────────╮   │ │ ╭────────────╮   │
                if(std::regex_search(b.str.cbegin() + i, b.str.cend(), filePathMatch, std::regex(R"(^("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>))"))) {
                    ElmCoords filePathCoords(sourceFilePaths[DBG_filePathIndex], includeLineNum, OG_i + match[0].length(), OG_i + match[0].length() + filePathMatch[0].length()); //FIXME set line number  //FIXME FIX ELEMENT WIDTH
                    i += filePathMatch[0].length();



                    // FIXME check if this actually works
                    // Catch up with the segment index of the current file
                    //! (If the include statament had comments or LCTs in it, OG_i and sgmi need to be updated to account for them)
                    //! At this stage, OG_i points to the first character of the include statement
                    //
                    //            OG_start   OG_i
                    //                  ▼     ▼
                    //     /* comment */int n;#includ/*cmt*//*cmt*/e "file.lmn"int var;/* comment 2 */
                    //     ╰──── 0 ────╯╰──── 1 ────╯╰─ 0 ─╯╰─ 0 ─╯╰─────── 1 ────────╯╰───── 0 ─────╯
                    //                  ╰─┬──╯╰──┬──╯              ╰─────┬────╯╰──┬───╯
                    //            lRemainder    lIncludeLen     rIncludeLen      rRemainder
                    //!                                         ▲  == includeLen after skipping middle segments


                    // Push left remainder if present
                    ulong lRemainder = OG_i - b.sgm[sgmi].OG_start;
                    if(lRemainder > 0) {
                        //FIXME PUSH FIRST HALF
                    }

                    // Skip left part if present (and check the other parts)
                    ulong includeLen = match[0].length() + filePathMatch[0].length();
                    ulong lIncludeLen = b.sgm[sgmi].len - lRemainder;
                    if(lIncludeLen > 0) {
                        OG_i       += lIncludeLen;
                        includeLen -= lIncludeLen;
                        ++sgmi;

                        // Skip middle segments without pushing them
                        while(sgmi < b.sgm.size() && b.sgm[sgmi].len < includeLen) {
                            if(!b.sgm[sgmi].isRemoved) {
                                includeLen -= b.sgm[sgmi].len;
                            }
                            OG_i += b.sgm[sgmi].len;
                            ++sgmi;
                        }

                        // Skip right part if present (and check right remainder)
                        if(sgmi < b.sgm.size() && includeLen > 0) {
                            OG_i += includeLen;

                            // Push right remainder if present
                            ulong rRemainder = b.sgm[sgmi].len - includeLen;
                            if(rRemainder > 0) {
                                //FIXME PUSH SECOND HALF
                            }
                        }
                    }

                    //! last segment is skipped regardless of the remainder length
                    ++sgmi;




                    // File path is present
                    if(filePathMatch[0].length() > 2) {
                        std::string rawIncludeFilePath = filePathMatch[0].str().substr(1, filePathMatch[0].length() - 2);  //! Include path as written in the source file

                        // If the included file is a standard module
                        if(filePathMatch[0].str()[0] == '<') {
                            //FIXME check name correctness and include the module
                            //TODO write this part without the check and copy parts to make the preprocessor work before standard modules are implemented
                            //TODO or maybe just check the name but don't include, since we already know what modules will be available
                        }

                        // If it is an actual file
                        else {
                            //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
                            //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
                            //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
                            // Calculate the actual file path
                            std::filesystem::path adjustedIncludeFilePath = std::filesystem::canonical(sourceFilePaths[DBG_filePathIndex]).parent_path() / rawIncludeFilePath;    //! Include path relative to the file the include statement was used in
                            std::string canonicalIncludeFilePath;                                                                                           //! Canonical version of the adjusted file path. No changes if file was not found
                            try { canonicalIncludeFilePath = std::filesystem::canonical(adjustedIncludeFilePath).string(); }
                            catch(std::filesystem::filesystem_error e) { canonicalIncludeFilePath = adjustedIncludeFilePath.string(); }

                            // Print an error if the file is a directory
                            if(std::filesystem::is_directory(canonicalIncludeFilePath)) {
                                printError(
                                    utils::ErrType::PREPROCESSOR,
                                    relevantCoords,
                                    filePathCoords,
                                    "Could not include the specified path: \"" + rawIncludeFilePath + "\" is a directory.\n" +
                                    "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + "."
                                );
                                exit(1);
                            }

                            // Print an error if the file cannot be opened
                            std::ifstream includeFile(canonicalIncludeFilePath);
                            if(!includeFile) {
                                printError(
                                    utils::ErrType::PREPROCESSOR,
                                    relevantCoords,
                                    filePathCoords,
                                    "Could not open file \"" + rawIncludeFilePath + "\": " + std::strerror(errno) + ".\n" +
                                    "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + ".\n" +
                                    "Make sure that the path is correct and the compiler has read access to the file."
                                );
                                exit(1);
                            }

                            // Copy file contents and segments
                            std::string fileContents = utils::readFile(includeFile);
                            SegmentedCleanSource preprocessedCode = loadSourceCode(fileContents, canonicalIncludeFilePath); //FIXME run concurrently
                            // r += preprocessedCode.first;
                            r.str += preprocessedCode.str;

                            // Push all the segments from the included file
                            for(ulong j = 0; j < preprocessedCode.sgm.size(); ++j) {
                                r.sgm.push_back(preprocessedCode.sgm[j]);
                            }
                        }
                    }

                    // Empty string
                    else {
                        utils::printError(
                            utils::ErrType::PREPROCESSOR,
                            relevantCoords,
                            filePathCoords,
                            "Empty file path in include statement.\n"
                            "A file path must be specified."
                        );
                        exit(1);
                    }
                }

                // String literal not found
                else {
                    utils::printError(
                        utils::ErrType::PREPROCESSOR,
                        relevantCoords,
                        ElmCoords(sourceFilePaths[DBG_filePathIndex], includeLineNum, i, i), //FIXME set line number
                        "Missing file path in include statement.\n"
                        "A valid string literal was expected, but could not be found."
                    );
                    exit(1); //FIXME move this to the printError function itself
                }

            }


            // If not, copy normal characters and increse index counters
            else {
                r.str += b.str[i];
                ++i;
                ++OG_i;

                // If the next segment has been reached, push and update segment index
                //! Deleted elements are pushed in the next iteration
                if(OG_i >= b.sgm[sgmi].OG_start + b.sgm[sgmi].len) {
                    r.sgm.push_back(b.sgm[sgmi]);
                    ++sgmi;
                }
            }
        }




        // Push remaining deleted segments
        while(i < b.sgm.size() && b.sgm[sgmi].isRemoved) {
            r.sgm.push_back(b.sgm[sgmi]);
            //! No need to update OG_i as it is not used anymore
            ++sgmi;
        }




        return r;
    }









    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED
    //TODO REMOVE IF UNUSED


    // //TODO check if multi-byte unicode characters can screw with the string detection
    // /**
    //  * @brief Parses the preprocessor directive that starts at index <index>.
    //  *     This function does NOT check if the attached data contains valid data, but it does check if it's of the correct type based on the detected directive.
    //  *     It also prints an error if the directive is not recognized.
    //  * @param b The string buffer that contains the directive.
    //  * @param index The index at which the directive starts.
    //  * @param DEBUG_curLine The current line number in the original file at which the directive starts.
    //  * @param DBG_filePath The path to the file the buffer string was read from.
    //  * @return
    //  */
    // ParsingResult parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DBG_filePath) {
    //     ParsingResult r;
    //     if(b[index] != '#') return r;
    //     r.trueValue  += '#';
    //     //r.finalValue += '#';  //! Copied from trueValue after parsing the name of the directive


    //     ulong i = index + 1;
    //     std::smatch matchRes;
    //     if(std::regex_search(std::string::const_iterator(b.begin() + i), std::string::const_iterator(b.end()), matchRes, std::regex(R"(^([a-zA-Z]|(\\\n))+)"))){

    //         // Find directive name
    //         r.trueValue += matchRes[0].str();
    //         r.finalValue = std::regex_replace(r.trueValue, std::regex(R"(\\\n)"), "");
    //         r.height += (r.trueValue.length() - r.finalValue.length()) / 2;     //! Find number of removed LCTs by comparing the lengths of the strings
    //         i += r.trueValue.length() - 1;                                      //! -1 to account for the '#' character


    //         // Check if it is valid and set the source element type
    //         if(r.finalValue == "#include") { //FIXME add glob patterns to include statements
    //             r.elmType = SourceElmType::DIRECTIVE_INCLUDE;
    //         }
    //         else if(r.finalValue == "#define") {
    //             r.elmType = SourceElmType::DIRECTIVE_DEFINE;
    //         }
    //         else {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DEBUG_curLine + r.height - 1, index, i - 1),
    //                 "Unknown preprocessor directive \"" + r.finalValue + "\"."
    //             );
    //             exit(1);
    //         }


    //         // Check if whitespace is present and skip it
    //         WhitespaceInfo wsCheck = countWhitespaceCharacters(b, i, DEBUG_curLine + r.height - 1, DBG_filePath);
    //         if(wsCheck.w > 0) {
    //             r.trueValue += b.substr(i, wsCheck.w);
    //             r.finalValue += " ";
    //             r.height += wsCheck.h - 1;
    //             i += wsCheck.w;
    //         }
    //         else {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DBG_filePath, DEBUG_curLine + r.height - 1, index, i),
    //                 "Missing whitespace after include statement.\n"
    //                 "The name of the directive and its definition must be separated by one or more whitespace characters."
    //             );
    //             exit(1);
    //         }


    //         // Parse string if the directive is an include statement
    //         if(r.elmType == SourceElmType::DIRECTIVE_INCLUDE && b[i]) {
    //             ParsingResult stringCheck = parseStrLiteral(b, i, DEBUG_curLine + r.height - 1, DBG_filePath);
    //             if(stringCheck.elmType == SourceElmType::STRING) {
    //                 r.finalValue += stringCheck.finalValue;
    //                 r.trueValue += stringCheck.trueValue;
    //                 r.height += stringCheck.height - 1;
    //                 i += stringCheck.trueValue.length();
    //                 //FIXME add standard module inclusion <>
    //             }
    //             else {
    //                 utils::printError(
    //                     utils::ErrType::PREPROCESSOR,
    //                     ElmCoords(DBG_filePath, DEBUG_curLine + r.height - 1, index, i),
    //                     "Missing file path in include statement.\n"
    //                     "A string literal was expected, but could not be found." //TODO check if this error works
    //                 );
    //                 exit(1);
    //             }
    //         }


    //         // Parse whole line if the directive is a define directive
    //         else if(r.elmType == SourceElmType::DIRECTIVE_DEFINE) {
    //             while(true) {
    //                 // Check line continuation token
    //                 if(isLct(b, i)) {
    //                     r.trueValue += "\\\n";
    //                     ++r.height;
    //                     i += 2;
    //                 }

    //                 // Check newline (terminates macro definitions)
    //                 else if(b[i] == '\n') {
    //                     r.finalValue += '\n';
    //                     r.trueValue += '\n';
    //                     ++r.height;
    //                     break;
    //                 }

    //                 // Check EOF (terminates macro definitions)
    //                 else if(b[i] == '\0') {
    //                     break;
    //                 }

    //                 // Normal characters (macro definition)
    //                 else {
    //                     r.finalValue += b[i];
    //                     r.trueValue += b[i];
    //                     ++i;
    //                 }
    //             }
    //         }

    //         // // Normal characters (part of the string)
    //         // else {
    //         //     r.trueValue  += b[i];
    //         //     r.finalValue += b[i];
    //         //     last = b[i];
    //         //     ++i;
    //         // }
    //     }


    //     return r;
    // }
}