#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>

#include "preprocessor.hpp"
#include "utils.hpp"



namespace pre {
    /**
     * @brief Reads a single source file and returns its contents as a string.
     *      Line continuation tokens are automatically recognized and replaced.
     *      Prints an error message if the file cannot be opened or doesn't exist.
     * @param fileName The path to the file
     * @return The contents of the file
     */
    static std::string readAndCheckFile(std::string fileName) {
        // Create file stream and print an error if the file cannot be opened
        std::ifstream f(fileName);
        if(!f) utils::printError(
            utils::ErrType::PREPROCESSOR,
            "Could not open file \"" + fileName + "\": " + std::strerror(errno) + ".\n" +
            "Current working directory is: " + std::string(std::filesystem::current_path()) + "."
        );

        // Read all the lines and return a string containing all of them
        std::string l, r;
        while(getline(f, l)) {
            r += l;
            // Remove line continuation tokens and merge lines if needed
            if(l[l.length() - 1] != '\\') r += "\n"; else r.resize(r.length() - 1);
        }
        return r;
    }




    /**
     * @brief Loads the source file using the provided options without processing any of it.
     * @param options The options
     * @return The contents of the source file as a string
     */
    std::string loadSourceFile(Options& options) {
        return readAndCheckFile(options.sourceFile);
    }




    /**
     * @brief Finds and processes preprocessor directives.
     * @param s The source file as a string
     * @return The full source code as a string, without any preprocessor directives
     */
    std::string processDirectives(std::string s) {
        std::string r;
        for(ulong i = 0; i < s.length();){
            if(s[i] == '\'') {
                utils::checkCharLiteral(s, i);
                std::cout << "CHAR" << s[i+1] << "\n";
                i += 3;
            }
            else if(s[i] == '"') {
                std::cout << "STRING" << s.substr(i, i + utils::computeStrLiteralLen(s, i) - 1) << "\n";
                i += utils::computeStrLiteralLen(s, i);
            }
            else ++i;
        }

        // return r;
        return s;
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
}