#pragma once
#include <string>
#include <mutex>

#include "ansi.hpp"
#include "ErrorCode.hpp"
#include "Command/CommandCoords.hpp"
#include "Preprocessor/ElmCoords.hpp"





struct ElmCoords;
namespace utils {
    extern std::mutex consoleLock;


    enum class ErrType:int {
        PREPROCESSOR,
        COMPILER
    };


    int getConsoleWidth();
    bool isDir(std::string const &path);

    std::string formatChar(char c, bool useColor = false);
    std::string shortenInteger(ulong n);

    void printErrorGeneric(ErrorCode errorCode,                                                                                  std::string const &message);
    void      printErrorCL(ErrorCode errorCode,                  cmd::ElmCoordsCL const &relPos, cmd::ElmCoordsCL const &errPos, std::string const &message, std::string const &fullCommand);
    void        printError(ErrorCode errorCode, ErrType errType,                                        ElmCoords const &errPos, std::string const &message);
    void        printError(ErrorCode errorCode, ErrType errType,        ElmCoords const &relPos,        ElmCoords const &errPos, std::string const &message);

    std::string readAndCheckFile(std::string const &fileName);
    std::string readFile(std::ifstream &f);
}