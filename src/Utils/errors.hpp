#pragma once
#include "ErrorCode.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Command/CommandCoords.hpp"




namespace utils {
    enum class ErrType:int {
        PREPROCESSOR,
        COMPILER
    };

    void printErrorGeneric(ErrorCode errorCode,                                                                                    std::string const &message);
    void      printErrorCL(ErrorCode errorCode,                  cmd::ElmCoordsCL const &_relPos, cmd::ElmCoordsCL const &_errPos, std::string const &message, std::string const &fullCommand);
    void        printError(ErrorCode errorCode, ErrType errType,                                       ElmCoords   const &_errPos, std::string const &message);
    void        printError(ErrorCode errorCode, ErrType errType,        ElmCoords const &_relPos,      ElmCoords   const &_errPos, std::string const &message);
}