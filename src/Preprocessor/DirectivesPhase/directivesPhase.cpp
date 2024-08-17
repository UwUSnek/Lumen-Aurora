#include "directivesPhase.hpp"








namespace pre {
    IntermediateCodeFormat startDirectivesPhase(IntermediateCodeFormat cleanCode, std::string filePath) {
        ulong i;
        while(i < cleanCode.elms.size()) {
            ++i; //TODO
        }
    }
















    // //TODO check if multi-byte unicode characters can screw with the string detection
    // /**
    //  * @brief Parses the preprocessor directive that starts at index <index>.
    //  *     This function does NOT check if the attached data contains valid data, but it does check if it's of the correct type based on the detected directive.
    //  *     It also prints an error if the directive is not recognized.
    //  * @param b The string buffer that contains the directive.
    //  * @param index The index at which the directive starts.
    //  * @param DEBUG_curLine The current line number in the original file at which the directive starts.
    //  * @param DEBUG_filePath The path to the file the buffer string was read from.
    //  * @return
    //  */
    // ParsingResult parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath) {
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
    //                 ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i - 1),
    //                 "Unknown preprocessor directive \"" + r.finalValue + "\"."
    //             );
    //             exit(1);
    //         }


    //         // Check if whitespace is present and skip it
    //         WhitespaceInfo wsCheck = countWhitespaceCharacters(b, i, DEBUG_curLine + r.height - 1, DEBUG_filePath);
    //         if(wsCheck.w > 0) {
    //             r.trueValue += b.substr(i, wsCheck.w);
    //             r.finalValue += " ";
    //             r.height += wsCheck.h - 1;
    //             i += wsCheck.w;
    //         }
    //         else {
    //             utils::printError(
    //                 utils::ErrType::PREPROCESSOR,
    //                 ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
    //                 "Missing whitespace after include statement.\n"
    //                 "The name of the directive and its definition must be separated by one or more whitespace characters."
    //             );
    //             exit(1);
    //         }


    //         // Parse string if the directive is an include statement
    //         if(r.elmType == SourceElmType::DIRECTIVE_INCLUDE && b[i]) {
    //             ParsingResult stringCheck = parseStrLiteral(b, i, DEBUG_curLine + r.height - 1, DEBUG_filePath);
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
    //                     ElmCoords(DEBUG_filePath, DEBUG_curLine + r.height - 1, index, i),
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