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
    void printError(ErrType errorType, std::string message) {
        std::cerr << ansi::fgRed << ansi::bold;
        switch(errorType) {
            case ErrType::COMMAND:      { std::cerr << "Could not parse terminal command:"; break; }
            case ErrType::PREPROCESSOR: { std::cerr << "Preprocessor error:";               break; }
            case ErrType::COMPILER:     { std::cerr << "Compilation error:";                break; }
        }
        std::cerr << ansi::reset << "\n";

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
        if(!f) utils::printError(
            utils::ErrType::PREPROCESSOR,
            "Could not open file \"" + fileName + "\": " + std::strerror(errno) + ".\n" +
            "Current working directory is: " + std::string(std::filesystem::current_path()) + "."
        );

        //FIXME move this to the detection fuctions
        //FIXME skip these automatically and return both versions (with and without)
        // Read all the lines and return a string containing all of them
        std::string l, r;
        while(getline(f, l)) {
            r += l;
            // Remove line continuation tokens and merge lines if needed
            if(l[l.length() - 1] != '\\') r += "\n"; else r.resize(r.length() - 1);
        }
        return r;
    }
}


