#include "ansi.hpp"








// Normal colors
std::string ansi::black = "";             std::string ansi::bold_black = "";              std::string ansi::fill_black = "";
std::string ansi::red = "";               std::string ansi::bold_red = "";                std::string ansi::fill_red = "";
std::string ansi::green = "";             std::string ansi::bold_green = "";              std::string ansi::fill_green = "";
std::string ansi::yellow = "";            std::string ansi::bold_yellow = "";             std::string ansi::fill_yellow = "";
std::string ansi::blue = "";              std::string ansi::bold_blue = "";               std::string ansi::fill_blue = "";
std::string ansi::magenta = "";           std::string ansi::bold_magenta = "";            std::string ansi::fill_magenta = "";
std::string ansi::cyan = "";              std::string ansi::bold_cyan = "";               std::string ansi::fill_cyan = "";
std::string ansi::white = "";             std::string ansi::bold_white = "";              std::string ansi::fill_white = "";

// High intensity colors
std::string ansi::bright_black = "";      std::string ansi::bold_bright_black = "";      std::string ansi::fill_bright_black = "";
std::string ansi::bright_red = "";        std::string ansi::bold_bright_red = "";        std::string ansi::fill_bright_red = "";
std::string ansi::bright_green = "";      std::string ansi::bold_bright_green = "";      std::string ansi::fill_bright_green = "";
std::string ansi::bright_yellow = "";     std::string ansi::bold_bright_yellow = "";     std::string ansi::fill_bright_yellow = "";
std::string ansi::bright_blue = "";       std::string ansi::bold_bright_blue = "";       std::string ansi::fill_bright_blue = "";
std::string ansi::bright_magenta = "";    std::string ansi::bold_bright_magenta = "";    std::string ansi::fill_bright_magenta = "";
std::string ansi::bright_cyan = "";       std::string ansi::bold_bright_cyan = "";       std::string ansi::fill_bright_cyan = "";
std::string ansi::bright_white = "";      std::string ansi::bold_bright_white = "";      std::string ansi::fill_bright_white = "";

// Lone formatting
std::string ansi::bold = "";         std::string ansi::boldOff = "";
std::string ansi::underline = "";    std::string ansi::underlineOff = "";
std::string ansi::inverse = "";      std::string ansi::inverseOff = "";

// Reset
std::string ansi::reset = "";








/**
 * @brief Enables the aliases of formatting ANSI sequences defined by the ansi.hpp header.
 */
void ansi::enableEscapes(){
    // Normal colors
    black         = "\033[0;30m";    bold_black           = "\033[0;1;30m";    fill_black          = "\033[40m";
    red           = "\033[0;31m";    bold_red             = "\033[0;1;31m";    fill_red            = "\033[41m";
    green         = "\033[0;32m";    bold_green           = "\033[0;1;32m";    fill_green          = "\033[42m";
    yellow        = "\033[0;33m";    bold_yellow          = "\033[0;1;33m";    fill_yellow         = "\033[43m";
    blue          = "\033[0;34m";    bold_blue            = "\033[0;1;34m";    fill_blue           = "\033[44m";
    magenta       = "\033[0;35m";    bold_magenta         = "\033[0;1;35m";    fill_magenta        = "\033[45m";
    cyan          = "\033[0;36m";    bold_cyan            = "\033[0;1;36m";    fill_cyan           = "\033[46m";
    white         = "\033[0;37m";    bold_white           = "\033[0;1;37m";    fill_white          = "\033[47m";

    // High intensity colors
    bright_black   = "\033[0;90m";    bold_bright_black   = "\033[0;1;90m";    fill_bright_black   = "\033[100m";
    bright_red     = "\033[0;91m";    bold_bright_red     = "\033[0;1;91m";    fill_bright_red     = "\033[101m";
    bright_green   = "\033[0;92m";    bold_bright_green   = "\033[0;1;92m";    fill_bright_green   = "\033[102m";
    bright_yellow  = "\033[0;93m";    bold_bright_yellow  = "\033[0;1;93m";    fill_bright_yellow  = "\033[103m";
    bright_blue    = "\033[0;94m";    bold_bright_blue    = "\033[0;1;94m";    fill_bright_blue    = "\033[104m";
    bright_magenta = "\033[0;95m";    bold_bright_magenta = "\033[0;1;95m";    fill_bright_magenta = "\033[105m";
    bright_cyan    = "\033[0;96m";    bold_bright_cyan    = "\033[0;1;96m";    fill_bright_cyan    = "\033[106m";
    bright_white   = "\033[0;97m";    bold_bright_white   = "\033[0;1;97m";    fill_bright_white   = "\033[107m";

    // Lone formatting
    bold       = "\033[1m";    boldOff      = "\033[21m";
    underline  = "\033[4m";    underlineOff = "\033[24m";
    inverse    = "\033[7m";    inverseOff   = "\033[27m";

    // Reset
    reset      = "\033[0m";

    //! Bold often uses a brighter shade of the same color
    //! Inverse swaps foreground and background colors
}








/**
 * @brief Disables the aliases of formatting ANSI sequences the ansi.hpp header defines by setting them to an empty string.
 *      This doesn't affect the actual ANSI sequences, which will still be available to the program and will be recognized by the console.
 */
void ansi::disableEscapes(){
    // Normal colors
    black         = "";    bold_black           = "";    fill_black          = "";
    red           = "";    bold_red             = "";    fill_red            = "";
    green         = "";    bold_green           = "";    fill_green          = "";
    yellow        = "";    bold_yellow          = "";    fill_yellow         = "";
    blue          = "";    bold_blue            = "";    fill_blue           = "";
    magenta       = "";    bold_magenta         = "";    fill_magenta        = "";
    cyan          = "";    bold_cyan            = "";    fill_cyan           = "";
    white         = "";    bold_white           = "";    fill_white          = "";

    // High intensity colors
    bright_black   = "";    bold_bright_black   = "";    fill_bright_black   = "";
    bright_red     = "";    bold_bright_red     = "";    fill_bright_red     = "";
    bright_green   = "";    bold_bright_green   = "";    fill_bright_green   = "";
    bright_yellow  = "";    bold_bright_yellow  = "";    fill_bright_yellow  = "";
    bright_blue    = "";    bold_bright_blue    = "";    fill_bright_blue    = "";
    bright_magenta = "";    bold_bright_magenta = "";    fill_bright_magenta = "";
    bright_cyan    = "";    bold_bright_cyan    = "";    fill_bright_cyan    = "";
    bright_white   = "";    bold_bright_white   = "";    fill_bright_white   = "";

    // Lone formatting
    bold       = "";    boldOff      = "";
    underline  = "";    underlineOff = "";
    inverse    = "";    inverseOff   = "";

    // Reset
    reset      = "";
}