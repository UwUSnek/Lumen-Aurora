#pragma once
#include <string>





struct ElmCoords;
namespace utils {
    enum class ErrType:int {
        COMMAND,
        PREPROCESSOR,
        COMPILER
    };

    void printError(ErrType errorType, ElmCoords elmCoords, std::string message);
    std::string readAndCheckFile(std::string fileName);
}




namespace ansi {
    static const std::string fgBlack   = "\033[30m";    static const std::string bgBlack   = "\033[40m";
    static const std::string fgRed     = "\033[31m";    static const std::string bgRed     = "\033[41m";
    static const std::string fgGreen   = "\033[32m";    static const std::string bgGreen   = "\033[42m";
    static const std::string fgYellow  = "\033[33m";    static const std::string bgYellow  = "\033[43m";
    static const std::string fgBlue    = "\033[34m";    static const std::string bgBlue    = "\033[44m";
    static const std::string fgMagenta = "\033[35m";    static const std::string bgMagenta = "\033[45m";
    static const std::string fgCyan    = "\033[36m";    static const std::string bgCyan    = "\033[46m";
    static const std::string fgWhite   = "\033[37m";    static const std::string bgWhite   = "\033[47m";

    static const std::string bold       = "\033[1m";    static const std::string boldOff      = "\033[21m";
    static const std::string underline  = "\033[4m";    static const std::string underlineOff = "\033[24m";
    static const std::string inverse    = "\033[7m";    static const std::string inverseOff   = "\033[27m";

    static const std::string reset      = "\033[0m";

    //! Bold often uses a brighter shade of the same color
    //! Inverse swaps foreground and background colors
}