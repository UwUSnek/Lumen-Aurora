#pragma once
#include "ErrorCode.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Command/CommandCoords.hpp"




namespace utils {
    enum class ErrType:int {
        PREPROCESSOR,
        COMPILER
    };

    void printErrorGeneric(ErrorCode errorCode,                                                                                    const std::string &message, const bool fatal);
    void      printErrorCL(ErrorCode errorCode,                  cmd::ElmCoordsCL const &_relPos, cmd::ElmCoordsCL const &_errPos, const std::string &message, const bool fatal, const std::string &fullCommand);
    void        printError(ErrorCode errorCode, ErrType errType,                                       ElmCoords   const &_errPos, const std::string &message, const bool fatal);
    void        printError(ErrorCode errorCode, ErrType errType,        ElmCoords const &_relPos,      ElmCoords   const &_errPos, const std::string &message, const bool fatal);
    void        printError(ErrorCode errorCode, ErrType errType,        ElmCoords const &_relPos,      ElmCoords   const &_errPos, const std::string &message, const bool fatal);
}