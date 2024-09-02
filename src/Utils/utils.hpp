#pragma once
#include <string>
#include "ansi.hpp"
#include "ErrorCode.hpp"
#include "Command/CommandCoords.hpp"
#include "Preprocessor/ElmCoords.hpp"





struct ElmCoords;
namespace utils {
    enum class ErrType:int {
        PREPROCESSOR,
        COMPILER
    };


    int getConsoleWidth();
    bool isDir(std::string path);

    std::string formatChar(char c, bool useColor = false);

    void printErrorGeneric(ErrorCode errorCode, std::string message);
    void printErrorCL(ErrorCode errorCode, cmd::ElmCoordsCL relPos, cmd::ElmCoordsCL errPos, std::string message, std::string fullCommand);
    void printError(ErrorCode errorCode, ErrType errType,                   ElmCoords errPos, std::string message);
    void printError(ErrorCode errorCode, ErrType errType, ElmCoords relPos, ElmCoords errPos, std::string message);

    std::string readAndCheckFile(std::string fileName);
    std::string readFile(std::ifstream &f);
}