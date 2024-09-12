#pragma once
#include <string>




namespace ansi {
    // Normal colors
    static const std::string black         = "\033[0;30m";    static const std::string bold_black           = "\033[0;1;30m";    static const std::string fill_black          = "\033[40m";
    static const std::string red           = "\033[0;31m";    static const std::string bold_red             = "\033[0;1;31m";    static const std::string fill_red            = "\033[41m";
    static const std::string green         = "\033[0;32m";    static const std::string bold_green           = "\033[0;1;32m";    static const std::string fill_green          = "\033[42m";
    static const std::string yellow        = "\033[0;33m";    static const std::string bold_yellow          = "\033[0;1;33m";    static const std::string fill_yellow         = "\033[43m";
    static const std::string blue          = "\033[0;34m";    static const std::string bold_blue            = "\033[0;1;34m";    static const std::string fill_blue           = "\033[44m";
    static const std::string magenta       = "\033[0;35m";    static const std::string bold_magenta         = "\033[0;1;35m";    static const std::string fill_magenta        = "\033[45m";
    static const std::string cyan          = "\033[0;36m";    static const std::string bold_cyan            = "\033[0;1;36m";    static const std::string fill_cyan           = "\033[46m";
    static const std::string white         = "\033[0;37m";    static const std::string bold_white           = "\033[0;1;37m";    static const std::string fill_white          = "\033[47m";

    // High intensity colors
    static const std::string bright_black   = "\033[0;90m";    static const std::string bold_bright_black   = "\033[0;1;90m";    static const std::string fill_bright_black   = "\033[100m";
    static const std::string bright_red     = "\033[0;91m";    static const std::string bold_bright_red     = "\033[0;1;91m";    static const std::string fill_bright_red     = "\033[101m";
    static const std::string bright_green   = "\033[0;92m";    static const std::string bold_bright_green   = "\033[0;1;92m";    static const std::string fill_bright_green   = "\033[102m";
    static const std::string bright_yellow  = "\033[0;93m";    static const std::string bold_bright_yellow  = "\033[0;1;93m";    static const std::string fill_bright_yellow  = "\033[103m";
    static const std::string bright_blue    = "\033[0;94m";    static const std::string bold_bright_blue    = "\033[0;1;94m";    static const std::string fill_bright_blue    = "\033[104m";
    static const std::string bright_magenta = "\033[0;95m";    static const std::string bold_bright_magenta = "\033[0;1;95m";    static const std::string fill_bright_magenta = "\033[105m";
    static const std::string bright_cyan    = "\033[0;96m";    static const std::string bold_bright_cyan    = "\033[0;1;96m";    static const std::string fill_bright_cyan    = "\033[106m";
    static const std::string bright_white   = "\033[0;97m";    static const std::string bold_bright_white   = "\033[0;1;97m";    static const std::string fill_bright_white   = "\033[107m";

    // Lone formatting
    static const std::string bold       = "\033[1m";    static const std::string boldOff      = "\033[21m";
    static const std::string underline  = "\033[4m";    static const std::string underlineOff = "\033[24m";
    static const std::string inverse    = "\033[7m";    static const std::string inverseOff   = "\033[27m";

    // Reset
    static const std::string reset      = "\033[0m";

    //! Bold often uses a brighter shade of the same color
    //! Inverse swaps foreground and background colors
}