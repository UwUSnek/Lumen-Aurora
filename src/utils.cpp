#include <string>
#include <iostream>
#include <regex>

#include "utils.hpp"






namespace utils {
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
}


