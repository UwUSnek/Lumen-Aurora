#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <tuple>

#include "preprocessor.hpp"
#include "ElmCoords.hpp"
#include "Utils/utils.hpp"
#include "CleanupPhase/cleanupPhase.hpp"
#include "IncludePhase/includePhase.hpp"



namespace pre {
    std::vector<std::string> sourceFilePaths;




    /**
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param options The options.
     * @return The contents of the source file as a StructuredSource. //FIXME fix all comments that referenc StructuredSource s
     */
    std::pair<std::string, LineReference> loadSourceCode(Options& options) {
        std::string s = utils::readAndCheckFile(options.sourceFile);
        return loadSourceCode(s, options.sourceFile);
    }




    /**
     * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
     * @param b The source code as a string.
     * @param filePath The path of the original source code file.
     * @return std::pair<std::string, LineReference>.
     */
    std::pair<std::string, LineReference> loadSourceCode(std::string b, std::string filePath) {
        sourceFilePaths.push_back(std::filesystem::canonical(filePath)); //TODO cache preprocessed files somewhere and add a function to chec for them before starting the preprocessor
        std::pair<std::string, LineReference> r1 = startCleanupPhase(b, filePath);
        std::pair<std::string, LineReference> r2 = startDirectivesPhase(r1, filePath);
        return r2;
    }







    // // Add elements to output array. If include statements are found, parse the files and include them recursively
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r) {
    //     auto &e = r.elms;
    //     for(ulong i = 0; i < e.size();) {

    //         // Replace include statements with the specified file
    //         // if(e[i].type == SourceElmType::DIRECTIVE) {

    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // if(i < e.size() - 1 && e[i +1].type == SourceElmType::STRING) {

    //             // }
    //             // else {
    //             //     utils::printError(
    //             //         utils::ErrType::PREPROCESSOR,
    //             //         ElmCoords(e[i].meta),
    //             //         "Missing file path in include statement. A string literal was expected, but could not be found."
    //             //     );
    //             //     exit(1);
    //             // }
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE
    //             // //FIXME MOVE THIS TO PARSING. PARSE THE STRING TOGETHER WITH THE DIRECTIVE

    //         //     continue;
    //         // }

    //         // Append anything else
    //         output.elms.push_back(e[i]);
    //         ++i;
    //     }
    // }









    //TODO add include backtrace with line and column number
    //TODO don't allow multi-byte characters inside of char literals

    //TODO check if multi-byte unicode characters can screw with the comment detection
}