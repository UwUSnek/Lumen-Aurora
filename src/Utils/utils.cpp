#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "utils.hpp"






namespace utils {
    static inline void printChar(char c) {
        if(c == ' ') std::cerr << "·";
        else std::cerr << c;
    }



    /**
     * @brief Prints an error to stderr, specifying the error type.
     *      This function doesn't stop the program.
     * @param errorType The type of the error
     * @param message The error message. This can contain multiple lines.
     *      One \n character is automatically added at the end of the string.
     */
    void printError(ErrType errType, ElmCoords elmCoords, std::string message) {
        // Print error type and location
        std::cerr << ansi::fgRed << ansi::bold;
        if(errType == ErrType::COMMAND) {
            std::cerr << "Could not parse terminal command:";
        }
        else {
            if(errType == ErrType::PREPROCESSOR) std::cerr << "Preprocessor";
            if(errType == ErrType::COMPILER)     std::cerr << "Compilation";
            std::cerr << " error:\n";
            if(elmCoords.filePath.length()) {
                std::cerr << "    File │ " << ansi::reset << std::filesystem::canonical(elmCoords.filePath) << ansi::fgRed << ansi::bold << "\n";
                std::cerr << "    Line │ " << ansi::reset << elmCoords.lineNum;
            }
        }




        // Print the offending lines and highlight the problem
        std::string s = readAndCheckFile(elmCoords.filePath);
        ulong curLine = elmCoords.lineNum;
        bool borderLineReached = false;
        ulong i = elmCoords.start;
        while(i > 0) {
            if(s[i] == '\n') {
                if(borderLineReached) break;
                borderLineReached = true;
                ++curLine;
            }
            --i;
        }
        if(i) ++i;  //! Skip \n character if found

        std::cerr << "\n\n" << ansi::fgBlack << ansi::bold << std::right << std::setw(8) << curLine << " │ " << ansi::reset << ansi::fgBlack;
        while(i < elmCoords.start) {
            printChar(s[i]);
            if(s[i] == '\n') {
                ++curLine;
                std::cerr << ansi::fgBlack << ansi::bold << std::right << std::setw(8) << curLine << " │ " << ansi::reset << ansi::fgBlack;
            }
            ++i;
        }

        std::cerr << ansi::fgMagenta << ansi::bold;
        while(i < elmCoords.end + 1) {
            printChar(s[i]);
            if(s[i] == '\n') {
                ++curLine;
                std::cerr << ansi::fgBlack << ansi::bold << std::right << std::setw(8) << curLine << " │ " << ansi::reset << ansi::fgBlack;
            }
            ++i;
        }

        std::cerr << ansi::reset << ansi::fgBlack;
        borderLineReached = false;
        while(s[i] != '\0') {
            printChar(s[i]);
            if(s[i] == '\n') {
                if(borderLineReached) break;
                borderLineReached = true;
                ++curLine;
                std::cerr << ansi::fgBlack << ansi::bold << std::right << std::setw(8) << curLine << " │ " << ansi::reset << ansi::fgBlack;
            }
            ++i;
        }
        std::cerr << ansi::reset;




        // Print the actual error after indenting it by 4 spaces
        std::cerr << "\n\n    " << std::regex_replace(message, std::regex("\n"), "\n    ") << "\n";
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


