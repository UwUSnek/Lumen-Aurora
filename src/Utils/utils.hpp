#pragma once
#include <string>
#include "ansi.hpp"
#include "ErrorCode.hpp"





struct ElmCoords;
namespace utils {
    enum class ErrType:int {
        COMMAND,
        PREPROCESSOR,
        COMPILER
    };


    int getConsoleWidth();

    std::string formatChar(char c, bool useColor = false);

    void printError(ErrorCode errorCode, ErrType errType,                   ElmCoords errPos, std::string message);
    void printError(ErrorCode errorCode, ErrType errType, ElmCoords relPos, ElmCoords errPos, std::string message);

    std::string readAndCheckFile(std::string fileName);
    std::string readFile(std::ifstream &f);
}