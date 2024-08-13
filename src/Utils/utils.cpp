#include <string>
#include <iostream>
#include <regex>
#include <fstream>
#include <cstring>
#include <filesystem>

#include "utils.hpp"
#include "Preprocessor/ElmCoords.hpp"






namespace utils {
    // Prints the formatted line indicator
    static inline void printLineNum(ulong n) {
        std::cerr << "\n" << ansi::fgBlack << ansi::bold << std::right << std::setw(8) << n << " │ " << ansi::reset << ansi::fgBlack;
    }


    // Prints a character.
    static inline void printChar(char c) {
        if(c == ' ') std::cerr << "·";
        else if(c == '\n');
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




        // Find the line in the original file and calculate the starting index of the preceding line
        std::string s = readAndCheckFile(elmCoords.filePath);
        ulong curLine = elmCoords.lineNum;
        int linesPrinted = 0;
        ulong i = elmCoords.start;
        while(i > 0) {
            if(s[i] == '\n') {
                if(linesPrinted > 0) break;
                linesPrinted++;
                ++curLine;
            }
            --i;
        }
        if(i) ++i;  //! Skip \n character if found


        // Print preceding line + preceding characters in the same line
        std::cerr << "\n\n";
        printLineNum(curLine);
        while(i < elmCoords.start) {
            printChar(s[i]);
            if(s[i] == '\n') {
                ++curLine;
                printLineNum(curLine);
            }
            ++i;
        }


        // Print offending substring
        std::cerr << ansi::fgMagenta << ansi::bold << ansi::underline;
        while(i < elmCoords.end + 1) {
            printChar(s[i]);
            if(s[i] == '\n') {
                ++curLine;
                printLineNum(curLine);
            }
            ++i;
        }


        // Print subsequent characters in the same line + subsequent line
        std::cerr << ansi::reset << ansi::fgBlack;
        linesPrinted = 0;
        while(s[i] != '\0') {
            printChar(s[i]);
            if(s[i] == '\n') {
                if(linesPrinted > 1) break;
                ++curLine;
                ++linesPrinted;
                if(linesPrinted < 2) printLineNum(curLine);
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


