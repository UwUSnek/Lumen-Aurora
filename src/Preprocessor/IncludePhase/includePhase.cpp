// #include <iostream>
// #include <regex>
// #include <fstream>
// #include <cstring>
// #include <filesystem>

// #include "Preprocessor/preprocessor.hpp"
// #include "includePhase.hpp"
// #include "Preprocessor/ElmCoords.hpp"
// #include "Preprocessor/CleanupPhase/cleanupPhase.hpp"






// //FIXME fix line references
// namespace pre {
//     LineReferencedSource startDirectivesPhase(std::pair<std::string, LineReferencedSource> &b, std::string DBG_filePath) {
//         std::string r;
//         LineReferencedSource lineRef;
//         lineRef.push_back(0);



//         std::string& bStr = b.first;        // Alias
//         LineReferencedSource& bRef = b.second;     // Alias
//         ulong lastLineStart = 0;            // TODO comment
//         ulong i = 0, curLine = 0;           // The current index and line number relative to the current file
//         ulong i2 = 0/*, curLine2 = 0*/;         // The current index and line number relative to the source code of the current file and all of the files included by it
//         while(i < bStr.length()) {

//             // Find original line using the current index and current line
//             while(i == bRef[curLine]) {  //TODO maybe make this loop into an actual function. there is one in the print function "lineReferencedDataToString" as well
//                 // if(curLine) r += "\n";
//                 ++curLine;
//                 lineRef.push_back(i2);
//                 // ++curLine2;
//             }

//             // Detect include directives and replace them with the contents of the file
//             std::smatch match;
//             if(std::regex_search(bStr.cbegin() + i, bStr.cend(), match, std::regex(R"(^#include[ \t]*)"))) {
//                 ElmCoords relevantCoords(DBG_filePath, 0, i, i + match[0].length()); //FIXME set line number
//                 i  += match[0].length();
//                 i2 += match[0].length();

//                 // Detect specified file path
//                 std::smatch filePathMatch;
//                 if(std::regex_search(bStr.cbegin() + i, bStr.cend(), filePathMatch, std::regex(R"(("(?:\\.|[^\\"])*?")|(<(?:\\.|[^\\>])*?>))"))) {
//                 //                                                                                │ ╰────────────╯   │ │ ╰────────────╯   │
//                 //                                                                                ╰────── file ──────╯ ╰───── module ─────╯
//                     ElmCoords filePathCoords(DBG_filePath, 0, i, i + filePathMatch[0].length()); //FIXME set line number
//                     i  += filePathMatch[0].length();
//                     i2 += filePathMatch[0].length();


//                     // File path is present
//                     if(filePathMatch[0].length() > 2) {
//                         std::string rawIncludeFilePath = filePathMatch[0].str().substr(1, filePathMatch[0].length() - 2);  //! Include path as written in the source file

//                         // If the included file is a standard module
//                         if(filePathMatch[0].str()[0] == '<') {
//                             //FIXME check name correctness and include the module
//                             //TODO write this part without the check and copy parts to make the preprocessor work before standard modules are implemented
//                             //TODO or maybe just check the name but don't include, since we already know what modules will be available
//                         }

//                         // If it is an actual file
//                         else {
//                             //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
//                             //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
//                             //FIXME MOVE ACTUAL FILE PATH CALCULATION AND ERRORS TO THE utils::readAndCheckFile FUNCTION
//                             // Calculate the actual file path
//                             std::filesystem::path adjustedIncludeFilePath = std::filesystem::canonical(DBG_filePath).parent_path() / rawIncludeFilePath;    //! Include path relative to the file the include statement was used in
//                             std::string canonicalIncludeFilePath;                                                                                           //! Canonical version of the adjusted file path. No changes if file was not found
//                             try { canonicalIncludeFilePath = std::filesystem::canonical(adjustedIncludeFilePath).string(); }
//                             catch(std::filesystem::filesystem_error e) { canonicalIncludeFilePath = adjustedIncludeFilePath.string(); }

//                             // Print an error if the file is a directory
//                             if(std::filesystem::is_directory(canonicalIncludeFilePath)) {
//                                 printError(
//                                     utils::ErrType::PREPROCESSOR,
//                                     relevantCoords,
//                                     filePathCoords,
//                                     "Could not include the specified path: \"" + rawIncludeFilePath + "\" is a directory.\n" +
//                                     "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + "."
//                                 );
//                                 exit(1);
//                             }

//                             // Print an error if the file cannot be opened
//                             std::ifstream includeFile(canonicalIncludeFilePath);
//                             if(!includeFile) {
//                                 printError(
//                                     utils::ErrType::PREPROCESSOR,
//                                     relevantCoords,
//                                     filePathCoords,
//                                     "Could not open file \"" + rawIncludeFilePath + "\": " + std::strerror(errno) + ".\n" +
//                                     "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + ".\n" +
//                                     "Make sure that the path is correct and the compiler has read access to the file."
//                                 );
//                                 exit(1);
//                             }

//                             // Copy file contents
//                             std::string fileContents = utils::readFile(includeFile);
//                             std::pair<std::string, LineReferencedSource> preprocessedCode = loadSourceCode(fileContents, canonicalIncludeFilePath); //FIXME run concurrently
//                             // r += preprocessedCode.first;
//                             std::string& ps = preprocessedCode.first;
//                             for(ulong j = 0; j < ps.length(); ++j) {
//                                 // while(i == bRef[curLine]) {  //TODO maybe make this loop into an actual function. there is one in the print function "lineReferencedDataToString" as well
//                                     // if(curLine) r += "\n";
//                                     // ++curLine;
//                                     // ++curLine2;
//                                 // }
//                                 r += ps[j];
//                                 ++i2;
//                                 //FIXME push reference without changing it
//                                 //FIXME it has already been calculated during the imported file's preprocessing phase
//                             }
//                         }
//                     }

//                     // Empty string
//                     else {
//                         utils::printError(
//                             utils::ErrType::PREPROCESSOR,
//                             relevantCoords,
//                             filePathCoords,
//                             "Empty file path in include statement.\n"
//                             "A file path must be specified"
//                         );
//                         exit(1);
//                     }
//                 }

//                 // String literal not found
//                 else {
//                     utils::printError(
//                         utils::ErrType::PREPROCESSOR,
//                         relevantCoords,
//                         ElmCoords(DBG_filePath, 0, i, i), //FIXME set line number
//                         "Missing file path in include statement.\n"
//                         "A valid string literal was expected, but could not be found."
//                     );
//                     exit(1); //FIXME move this to the printError function itself
//                 }
//             }




//             // Copy normal characters
//             else {
//                 r += bStr[i];
//                 ++i;
//             }
//         }
//         //sourceFilePaths.push_back(std::filesystem::canonical(options.sourceFile)); //TODO cache preprocessed files somewhere and add a function to chec for them before starting the preprocessor





//         // ulong i = 0;
//         // while(i < b.elms.size()) {

//         //     // If the element is arbitrary code (the only type that can contain directives)
//         //     if(b[i].t == ICF_ElmType::OTHER) {

//         //         // If it contains a directive
//         //         std::smatch match;
//         //         if(std::regex_match(b[i].s, match, std::regex(R"(^(.*?)#([a-zA-Z_][a-zA-Z_0-9]*)(.*)$)"))) {
//         //             //                                          " ╰ 1 ╯ ╰────────── 2 ─────────╯╰3 ╯ "

//         //             // Find the directive value and preceding characters in both the raw and clean strings
//         //             std::smatch OG_match;
//         //             std::regex_match(b[i].OG_s, OG_match, std::regex(R"(^((?:.|\n)*?)#((?:\\\n)?[a-zA-Z_](?:[a-zA-Z_0-9]|\\\n)*)((.|\n)*)$)"));
//         //             //                                                   │╰──────╯  │ │╰──────╯          ╰───────────────────╯ │╰── 3 ──╯
//         //             //                                                 " ╰─── 1 ────╯ ╰────────────────── 2 ───────────────────╯          "

//         //             // Push preceding characters if present
//         //             if(match[1].length()) {
//         //                 r.elms.push_back(ICF_Elm(
//         //                     b[i].t,                                             // Copy old type
//         //                     match[1],                                           // Use the match as string value
//         //                     OG_match[1],                                        // Use the OG match as OG string value
//         //                     b[i].OG_lineNum,                                    // Copy old line number
//         //                     (OG_match[1].length() - match[1].length()) / 2,     // Calculate the height using the length difference between the two matches (each \n is 2 +length)
//         //                     b[i].OG_start                                       // Copy old starting index
//         //                 ));
//         //             }

//         //             // Parse directive and update counter
//         //             std::cout << "found " << match[2] << "\n";
//         //             if(match[2] == "include") {
//         //                 processInclude(r, b, i, match, OG_match, DBG_filePath);
//         //                 i += 2;
//         //             }
//         //             else if(match[2] == "define") {
//         //                 // processDefine();
//         //                 //FIXME increase i
//         //             }
//         //             else {
//         //                 // processMacroCall();
//         //                 //FIXME increase i
//         //                 //TODO check if ( is the next character or element
//         //                 //TODO     If not there, the directive name is unknown. if found, save it in the output value and leave it for the next phase
//         //                 //TODO     ^ specify that macro calls need parameters and that they are passed with () after its name
//         //             }
//         //         }


//         //         // If not
//         //         else {
//         //             // Push element as-is
//         //             r.elms.push_back(b[i]);
//         //             ++i;
//         //         }
//         //     }




//         //     // If not
//         //     else {
//         //         // Push element as-is
//         //         r.elms.push_back(b[i]);
//         //         ++i;
//         //     }
//         // }

//         return std::pair<std::string, LineReferencedSource>(r, lineRef); //FIXME actually return the output and not a copy of the input
//         // return b;
//     }















//     // /**
//     //  * @brief Parses and processes an include statement, appending all the elements of the new file to <r>.
//     //  * @param r The output global buffer.
//     //  * @param b The buffer that contains the elements of the current file.
//     //  * @param i The index of the current element.
//     //  * @param match The match results of the current element.
//     //  * @param OG_match The match results of the raw value of the current element.
//     //  * @param DBG_filePath The path to the current file
//     //  * @return ulong The number of elements to skip, including the current one.
//     //  */
//     // //FIXME test all error output of this function extensively
//     // void processInclude(IntermediateCodeFormat &r, IntermediateCodeFormat &b, ulong i, std::smatch &match, std::smatch &OG_match, std::string DBG_filePath){

//     //     // Missing file path (junk after "#include")
//     //     if(match[3].length()) {
//     //         utils::printError(
//     //             utils::ErrType::PREPROCESSOR,
//     //             ElmCoords(DBG_filePath, b[i].OG_lineNum, b[i].OG_start, b[i].OG_start + OG_match[1].length() + OG_match[2].length() + 1), //💀
//     //             "Missing file path in include statement.\n"
//     //             "A string literal was expected, but could not be found."
//     //         );
//     //         exit(1);
//     //     }


//     //     // Missing file path (no string literal element)
//     //     else if(b[++i].t != ICF_ElmType::STRING) {
//     //         utils::printError(
//     //             utils::ErrType::PREPROCESSOR,
//     //             ElmCoords(DBG_filePath, b[i].OG_lineNum, b[i - 1].OG_start, b[i].OG_start + 1),
//     //             "Missing file path in include statement.\n"
//     //             "A string literal was expected, but could not be found."
//     //         );
//     //         exit(1);
//     //     }


//     //     // Missing file name (empty string)
//     //     else if(b[i].s.length() <= 2) {
//     //         utils::printError(
//     //             utils::ErrType::PREPROCESSOR,
//     //             ElmCoords(DBG_filePath, b[i].OG_lineNum, b[i - 1].OG_start + OG_match[1].length(), b[i].OG_start + 1),
//     //             "Empty file path in include statement.\n"
//     //             "A file path must be specified"
//     //         );
//     //         exit(1);
//     //     }


//     //     // File path and whitespace are present
//     //     //FIXME add glob patterns
//     //     //FIXME add standard module includes
//     //     else {
//     //         // Calculate the actual file path
//     //         std::string rawIncludeFilePath = b[i].s.substr(1, b[i].s.length() - 2);                                                         //! Include path as written in the source file
//     //         std::filesystem::path adjustedIncludeFilePath = std::filesystem::canonical(DBG_filePath).parent_path() / rawIncludeFilePath;    //! Include path relative to the file the include statement was used in
//     //         std::string canonicalIncludeFilePath;                                                                                           //! Canonical version of the adjusted file path. No changes if file was not found
//     //         try { canonicalIncludeFilePath = std::filesystem::canonical(adjustedIncludeFilePath).string(); }
//     //         catch(std::filesystem::filesystem_error e) { canonicalIncludeFilePath = adjustedIncludeFilePath.string(); }
//     //         std::ifstream includeFile(canonicalIncludeFilePath);

//     //         // Print an error if the file cannot be opened
//     //         if(!includeFile) {
//     //             printError(
//     //                 utils::ErrType::PREPROCESSOR,
//     //                 ElmCoords(DBG_filePath, b[i - 1].OG_lineNum, b[i].OG_start + OG_match[1].length(), b[i].OG_start + b[i].OG_s.length()),
//     //                 "Could not open file \"" + rawIncludeFilePath + "\": " + std::strerror(errno) + ".\n" +
//     //                 "File path was interpreted as: " + ansi::white + "\"" + canonicalIncludeFilePath + "\"" + ansi::reset + ".\n" +
//     //                 "Make sure that the path is correct and the compiler has read access to the file."
//     //             );
//     //             exit(1);
//     //         }

//     //         // If it can be opened and read
//     //         else {
//     //             // Read all the lines and save them in a string
//     //             std::string l, includeFileStr;
//     //             while(getline(includeFile, l)) {
//     //                 includeFileStr += l;
//     //                 includeFileStr += '\n';
//     //             }

//     //             // Preprocess the string as a regular source file and add it to the output list (without actually compiling it)
//     //             IntermediateCodeFormat includeFileCode1 = startCleanupPhase(includeFileStr,      rawIncludeFilePath);
//     //             IntermediateCodeFormat includeFileCode2 = startDirectivesPhase(includeFileCode2, rawIncludeFilePath);
//     //             r.elms.insert(r.elms.end(), includeFileCode2.elms.begin(), includeFileCode2.elms.end());
//     //         }
//     //     }
//     // }





































//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED
//     //TODO REMOVE IF UNUSED


//     // //TODO check if multi-byte unicode characters can screw with the string detection
//     // /**
//     //  * @brief Parses the preprocessor directive that starts at index <index>.
//     //  *     This function does NOT check if the attached data contains valid data, but it does check if it's of the correct type based on the detected directive.
//     //  *     It also prints an error if the directive is not recognized.
//     //  * @param b The string buffer that contains the directive.
//     //  * @param index The index at which the directive starts.
//     //  * @param DEBUG_curLine The current line number in the original file at which the directive starts.
//     //  * @param DBG_filePath The path to the file the buffer string was read from.
//     //  * @return
//     //  */
//     // ParsingResult parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DBG_filePath) {
//     //     ParsingResult r;
//     //     if(b[index] != '#') return r;
//     //     r.trueValue  += '#';
//     //     //r.finalValue += '#';  //! Copied from trueValue after parsing the name of the directive


//     //     ulong i = index + 1;
//     //     std::smatch matchRes;
//     //     if(std::regex_search(std::string::const_iterator(b.begin() + i), std::string::const_iterator(b.end()), matchRes, std::regex(R"(^([a-zA-Z]|(\\\n))+)"))){

//     //         // Find directive name
//     //         r.trueValue += matchRes[0].str();
//     //         r.finalValue = std::regex_replace(r.trueValue, std::regex(R"(\\\n)"), "");
//     //         r.height += (r.trueValue.length() - r.finalValue.length()) / 2;     //! Find number of removed LCTs by comparing the lengths of the strings
//     //         i += r.trueValue.length() - 1;                                      //! -1 to account for the '#' character


//     //         // Check if it is valid and set the source element type
//     //         if(r.finalValue == "#include") { //FIXME add glob patterns to include statements
//     //             r.elmType = SourceElmType::DIRECTIVE_INCLUDE;
//     //         }
//     //         else if(r.finalValue == "#define") {
//     //             r.elmType = SourceElmType::DIRECTIVE_DEFINE;
//     //         }
//     //         else {
//     //             utils::printError(
//     //                 utils::ErrType::PREPROCESSOR,
//     //                 ElmCoords(DBG_filePath, DEBUG_curLine + r.height - 1, index, i - 1),
//     //                 "Unknown preprocessor directive \"" + r.finalValue + "\"."
//     //             );
//     //             exit(1);
//     //         }


//     //         // Check if whitespace is present and skip it
//     //         WhitespaceInfo wsCheck = countWhitespaceCharacters(b, i, DEBUG_curLine + r.height - 1, DBG_filePath);
//     //         if(wsCheck.w > 0) {
//     //             r.trueValue += b.substr(i, wsCheck.w);
//     //             r.finalValue += " ";
//     //             r.height += wsCheck.h - 1;
//     //             i += wsCheck.w;
//     //         }
//     //         else {
//     //             utils::printError(
//     //                 utils::ErrType::PREPROCESSOR,
//     //                 ElmCoords(DBG_filePath, DEBUG_curLine + r.height - 1, index, i),
//     //                 "Missing whitespace after include statement.\n"
//     //                 "The name of the directive and its definition must be separated by one or more whitespace characters."
//     //             );
//     //             exit(1);
//     //         }


//     //         // Parse string if the directive is an include statement
//     //         if(r.elmType == SourceElmType::DIRECTIVE_INCLUDE && b[i]) {
//     //             ParsingResult stringCheck = parseStrLiteral(b, i, DEBUG_curLine + r.height - 1, DBG_filePath);
//     //             if(stringCheck.elmType == SourceElmType::STRING) {
//     //                 r.finalValue += stringCheck.finalValue;
//     //                 r.trueValue += stringCheck.trueValue;
//     //                 r.height += stringCheck.height - 1;
//     //                 i += stringCheck.trueValue.length();
//     //                 //FIXME add standard module inclusion <>
//     //             }
//     //             else {
//     //                 utils::printError(
//     //                     utils::ErrType::PREPROCESSOR,
//     //                     ElmCoords(DBG_filePath, DEBUG_curLine + r.height - 1, index, i),
//     //                     "Missing file path in include statement.\n"
//     //                     "A string literal was expected, but could not be found." //TODO check if this error works
//     //                 );
//     //                 exit(1);
//     //             }
//     //         }


//     //         // Parse whole line if the directive is a define directive
//     //         else if(r.elmType == SourceElmType::DIRECTIVE_DEFINE) {
//     //             while(true) {
//     //                 // Check line continuation token
//     //                 if(isLct(b, i)) {
//     //                     r.trueValue += "\\\n";
//     //                     ++r.height;
//     //                     i += 2;
//     //                 }

//     //                 // Check newline (terminates macro definitions)
//     //                 else if(b[i] == '\n') {
//     //                     r.finalValue += '\n';
//     //                     r.trueValue += '\n';
//     //                     ++r.height;
//     //                     break;
//     //                 }

//     //                 // Check EOF (terminates macro definitions)
//     //                 else if(b[i] == '\0') {
//     //                     break;
//     //                 }

//     //                 // Normal characters (macro definition)
//     //                 else {
//     //                     r.finalValue += b[i];
//     //                     r.trueValue += b[i];
//     //                     ++i;
//     //                 }
//     //             }
//     //         }

//     //         // // Normal characters (part of the string)
//     //         // else {
//     //         //     r.trueValue  += b[i];
//     //         //     r.finalValue += b[i];
//     //         //     last = b[i];
//     //         //     ++i;
//     //         // }
//     //     }


//     //     return r;
//     // }
// }