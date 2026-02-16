#pragma once
#include "Main/ErrorCode.hpp"
#include "Preprocessor/ElmCoords.hpp"
#include "Command/CommandCoords.hpp"




namespace utils {
    extern std::mutex  errorMutex;
    extern bool        errorPresent;
    extern std::string errorMessage;

    void storeErrorMessage(const std::string_view &msg);
    std::optional<std::string> getErrorMessage();

    enum class ErrType:int {
        PREPROCESSOR,
        COMPILER
    };

    std::string getErrTypeName(ErrType type);

    void printErrorGeneric(ErrorCode errorCode,                                                                                    const std::string &message, const bool fatal);
    void      printErrorCL(ErrorCode errorCode,                  cmd::ElmCoordsCL const &_relPos, cmd::ElmCoordsCL const &_errPos, const std::string &message, const bool fatal, const std::string &fullCommand);
    void        printError(ErrorCode errorCode, ErrType errType,                                       ElmCoords   const &_errPos, const std::string &message, const bool fatal);
    void        printError(ErrorCode errorCode, ErrType errType,        ElmCoords const &_relPos,      ElmCoords   const &_errPos, const std::string &message, const bool fatal);
    void        printError(ErrorCode errorCode, ErrType errType,        ElmCoords const &_relPos,      ElmCoords   const &_errPos, const std::string &message, const bool fatal);
}