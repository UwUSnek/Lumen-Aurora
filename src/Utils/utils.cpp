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
        std::cerr << ansi::reset << ansi::bold_black << "\n" << std::right << std::setw(8) << n << " │ " << ansi::black;
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
     *      The error message will be colored red and displayed as bold. ansi::reset will reset to bold red.
     */
    void printError(ErrType errType, ElmCoords elmCoords, std::string message) {
        std::cerr << ansi::bold_red;
        if(errType == ErrType::COMMAND) {

            // Print error type
            std::cerr << "Could not parse terminal command:";
        }
        else {
            // Print error type and location
            if(errType == ErrType::PREPROCESSOR) std::cerr << "Preprocessor";
            if(errType == ErrType::COMPILER)     std::cerr << "Compilation";
            std::cerr << " error:\n";


            // Find the line in the original file and calculate the starting index of the preceding line
            std::string s = readAndCheckFile(elmCoords.filePath);
            ulong curLine = elmCoords.lineNum;
            int linesPrinted = 0;
            ulong i = elmCoords.start;
            while(i > 0) {
                if(s[i] == '\n') {
                    if(linesPrinted > 0) break;
                    linesPrinted++;
                    --curLine;
                }
                --i;
            }
            if(i) ++i;  //! Skip \n character if found


            // Print location
            ulong elmHeight = std::count(s.c_str() + elmCoords.start, s.c_str() + elmCoords.end, '\n');
            if(elmCoords.filePath.length()) {
                std::cerr << "    File │ " << ansi::reset << std::filesystem::canonical(elmCoords.filePath) << ansi::bold_red << "\n";
                std::cerr << "    Line │ " << ansi::reset;
                if(elmHeight == 0) std::cerr << elmCoords.lineNum;
                else               std::cerr << "From " << elmCoords.lineNum << " to " << elmCoords.lineNum + elmHeight;
            }


            // Print preceding line + preceding characters in the same line
            std::cerr << "\n";
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
            std::cerr << ansi::bold_magenta << ansi::underline;
            while(i < elmCoords.end + 1) {
                printChar(s[i]);
                if(s[i] == '\n') {
                    ++curLine;
                    printLineNum(curLine);
                    std::cerr << ansi::bold_magenta << ansi::underline;  //! Re-set custom color after line indicator
                }
                ++i;
            }


            // Print subsequent characters in the same line + subsequent line
            std::cerr << ansi::black;
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
        }




        // Print the actual error after indenting it by 4 spaces
        std::cerr << ansi::bold_red << "\n\n    " << std::regex_replace(std::regex_replace(message, std::regex("\n"), "\n    "), std::regex("\033\\[0m"), ansi::bold_red) << "\n";
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


