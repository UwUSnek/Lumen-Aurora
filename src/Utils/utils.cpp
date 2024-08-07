#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "utils.hpp"






namespace utils {
    /**
     * @brief Prints an error to stderr, specifying the error type.
     *      This function doesn't stop the program.
     * @param errorType The type of the error
     * @param message The error message. This can contain multiple lines.
     *      One \n character is automatically added at the end of the string.
     */
    void printError(ErrType errType, ElmCoords &elmCoords, std::string message) {
        // Print error type and location
        std::cerr << ansi::fgRed << ansi::bold;
        if(errType == ErrType::COMMAND) {
            std::cerr << "Could not parse terminal command:";
        }
        else {
            if(errType == ErrType::PREPROCESSOR) std::cerr << "Preprocessor";
            if(errType == ErrType::COMPILER)     std::cerr << "Compilation";
            std::cerr << " error:\n    File: \"" << elmCoords.filePath << "\"\n    Line: " + elmCoords.lineNum;
        }
        std::cerr << ansi::reset << "\n";


        // Print the actual error after indenting it by 4 spaces
        std::cerr << "    " << std::regex_replace(message, std::regex("\n"), "\n    ") << "\n";
    }








    /**
     * @brief Reads a single source file and returns its contents as a string.
     *     Prints an error message if the file cannot be opened or doesn't exist.
     * @param fileName The path to the file
     * @return The contents of the file
     */
    std::string readAndCheckFile(std::string fileName) {
        // Create file stream and print an error if the file cannot be opened
        std::ifstream f(fileName);
        if(!f) printError(
            utils::ErrType::PREPROCESSOR,
            ElmCoords(),
            "Could not open file \"" + fileName + "\": " + std::strerror(errno) + ".\n" +
            "Current working directory is: " + std::string(std::filesystem::current_path()) + "."
        );

        // Read all the lines and return a string containing all of them
        std::string l, r;
        while(getline(f, l)) {
            r += l;
            r += '\n';
        }
        return r;
    }
}


