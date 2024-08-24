#pragma once
#include <string>
#include "ansi.hpp"





struct ElmCoords;
namespace utils {
    enum class ErrType:int {
        COMMAND,
        PREPROCESSOR,
        COMPILER
    };

    std::string formatChar(char c, bool useColor = false);

    void printError(ErrType errType,                   ElmCoords errPos, std::string message);
    void printError(ErrType errType, ElmCoords relPos, ElmCoords errPos, std::string message);

    std::string readAndCheckFile(std::string fileName);
    std::string readFile(std::ifstream &f);
}