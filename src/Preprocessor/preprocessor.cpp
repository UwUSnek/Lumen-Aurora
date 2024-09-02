#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>
#include <tuple>

#include "preprocessor.hpp"
#include "ElmCoords.hpp"
#include "Utils/utils.hpp"
#include "LCTsPhase/LCTsPhase.hpp"
#include "CleanupPhase/cleanupPhase.hpp"
#include "IncludePhase/includePhase.hpp"



namespace pre {
    /**
     * @brief The list of included source files in the order in which they were discovered.
     *      The source file passed through the command line is identified by the first element.
     *      All the paths saved in this vector are canonical paths.
     */
    std::vector<std::string> sourceFilePaths;






    SegmentedCleanSource loadSourceCode(std::string filePath) {
        std::string s = utils::readAndCheckFile(filePath);
        return loadSourceCode(s, filePath);
    }




    /**
     * @brief Load the source code, including all of the files included by it and processes any preprocessor directives.
     * @param b The source code as a string.
     * @param filePath The path of the original source code file.
     * @return The contents of the source file as a SegmentedCleanSource.
     */
    SegmentedCleanSource loadSourceCode(std::string s, std::string filePath) {
        sourceFilePaths.push_back(filePath); //TODO cache preprocessed files somewhere and add a function to chec for them before starting the preprocessor
        //FIXME ^automatically fish up cached files if found. loop through them (for now)
        //FIXME                                               ^ use a hash map to save the paths of the preprocessed files

        //FIXME CHECK CIRCULAR DEPENDENCIES
        //FIXME SAFE INCLUDE STACK

        SegmentedCleanSource r1 = startLCTsPhase(s, sourceFilePaths.size() - 1);
        SegmentedCleanSource r2 = startCleanupPhase(r1);
        SegmentedCleanSource r3 = startIncludePhase(r2);
        return r3;
        // return r2;
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