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
#include "DirectivesPhase/directivesPhase.hpp"



namespace pre {

    /**
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param options The options.
     * @return The contents of the source file as a StructuredSource.
     */
    IntermediateCodeFormat loadSourceCode(Options& options) {
        IntermediateCodeFormat output;
        loadSourceCode_loop(options.sourceFile, options, output);
        return output;
    }








    //TODO add element metadata
    /**
     * @brief Loads the source file, including all of the files included by it and processes any preprocessor directives.
     * @param filePath The path to the source file.
     * @param options The options.
     * @return The contents of the source file as a StructuredSource.
     */
    void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output) { //TODO use options or remove the parameter if not needed
        std::string s = utils::readAndCheckFile(filePath);

        IntermediateCodeFormat r1 = startCleanupPhase(s, filePath);
        IntermediateCodeFormat r2 = startDirectivesPhase(r1, filePath);
        output = r2;
        // mergeSourceElements(output, r);
    }








    // Add elements to output array. If include statements are found, parse the files and include them recursively
    void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r) {
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

    //TODO check if multi-byte unicode characters can screw with the comment detection
}