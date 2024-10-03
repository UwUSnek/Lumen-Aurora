#pragma once
#include <string>




namespace ansi {

    // Normal colors
    extern std::string black;             extern std::string bold_black;              extern std::string fill_black;
    extern std::string red;               extern std::string bold_red;                extern std::string fill_red;
    extern std::string green;             extern std::string bold_green;              extern std::string fill_green;
    extern std::string yellow;            extern std::string bold_yellow;             extern std::string fill_yellow;
    extern std::string blue;              extern std::string bold_blue;               extern std::string fill_blue;
    extern std::string magenta;           extern std::string bold_magenta;            extern std::string fill_magenta;
    extern std::string cyan;              extern std::string bold_cyan;               extern std::string fill_cyan;
    extern std::string white;             extern std::string bold_white;              extern std::string fill_white;

    // High intensity colors
    extern std::string bright_black;      extern std::string bold_bright_black;      extern std::string fill_bright_black;
    extern std::string bright_red;        extern std::string bold_bright_red;        extern std::string fill_bright_red;
    extern std::string bright_green;      extern std::string bold_bright_green;      extern std::string fill_bright_green;
    extern std::string bright_yellow;     extern std::string bold_bright_yellow;     extern std::string fill_bright_yellow;
    extern std::string bright_blue;       extern std::string bold_bright_blue;       extern std::string fill_bright_blue;
    extern std::string bright_magenta;    extern std::string bold_bright_magenta;    extern std::string fill_bright_magenta;
    extern std::string bright_cyan;       extern std::string bold_bright_cyan;       extern std::string fill_bright_cyan;
    extern std::string bright_white;      extern std::string bold_bright_white;      extern std::string fill_bright_white;

    // Lone formatting
    extern std::string bold;         extern std::string boldOff;
    extern std::string underline;    extern std::string underlineOff;
    extern std::string inverse;      extern std::string inverseOff;

    // Reset
    extern std::string reset;





    // Control functions
    void enableEscapes();
    void disableEscapes();
}