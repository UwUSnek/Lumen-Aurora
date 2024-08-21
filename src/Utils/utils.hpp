#pragma once
#include <string>





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
}




namespace ansi {
    static const std::string black   = "\033[0;30m";    static const std::string bold_black   = "\033[0;1;30m";    static const std::string fill_black   = "\033[40m";
    static const std::string red     = "\033[0;31m";    static const std::string bold_red     = "\033[0;1;31m";    static const std::string fill_red     = "\033[41m";
    static const std::string green   = "\033[0;32m";    static const std::string bold_green   = "\033[0;1;32m";    static const std::string fill_green   = "\033[42m";
    static const std::string yellow  = "\033[0;33m";    static const std::string bold_yellow  = "\033[0;1;33m";    static const std::string fill_yellow  = "\033[43m";
    static const std::string blue    = "\033[0;34m";    static const std::string bold_blue    = "\033[0;1;34m";    static const std::string fill_blue    = "\033[44m";
    static const std::string magenta = "\033[0;35m";    static const std::string bold_magenta = "\033[0;1;35m";    static const std::string fill_magenta = "\033[45m";
    static const std::string cyan    = "\033[0;36m";    static const std::string bold_cyan    = "\033[0;1;36m";    static const std::string fill_cyan    = "\033[46m";
    static const std::string white   = "\033[0;37m";    static const std::string bold_white   = "\033[0;1;37m";    static const std::string fill_white   = "\033[47m";

    static const std::string bold       = "\033[1m";    static const std::string boldOff      = "\033[21m";
    static const std::string underline  = "\033[4m";    static const std::string underlineOff = "\033[24m";
    static const std::string inverse    = "\033[7m";    static const std::string inverseOff   = "\033[27m";

    static const std::string reset      = "\033[0m";

    //! Bold often uses a brighter shade of the same color
    //! Inverse swaps foreground and background colors
}