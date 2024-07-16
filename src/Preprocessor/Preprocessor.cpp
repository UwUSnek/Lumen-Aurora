#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>

#include "Preprocessor.hpp"
#include "utils.hpp"



namespace pre {
    /**
     * @brief Reads a single source file and returns its contents as a string.
     *        Prints an error message if the file cannot be opened or doesn't exist.
     * @param fileName The path to the file
     * @return The contents of the file
     */
    static std::string readAndCheckFile(std::string fileName) {
        std::ifstream f(fileName);
        if(!f) utils::printError(
            utils::ErrType::PREPROCESSOR,
            "Could not open file \"" + fileName + "\": " + std::strerror(errno) + ".\n" +
            "Current working directory is: " + std::string(std::filesystem::current_path()) + "."
        );

        std::string l, r;
        while(getline(f, l)) r += l + "\n";
        return r;
    }




    /**
     * @brief Loads the source file using the provided options without processing any of it
     * @param options The options
     * @return The contents of the source file as a string
     */
    std::string loadSourceFile(Options& options) {
        return readAndCheckFile(options.sourceFile);
    }




    /**
     * @brief Finds and processes include directives
     * @param sourceFile The source file as a string
     * @return The full source code as a string
     */
    std::string processIncludes(std::string sourceFile) {
        return sourceFile;
    }
}