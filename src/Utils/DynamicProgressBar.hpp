#pragma once
#include <string>



struct DynamicProgressBar {
    // static int w, h;    // The width and height of the output console
    // int x, y;           // The position of the top-left corner in the output console
    std::string progressColor;
    std::string missingColor;

    ulong progress;     // The current progress in units
    ulong total;        // The total units of progress required to reach 100%


public:
    DynamicProgressBar(ulong total, std::string _progressColor, std::string _missingColor) :
        progress(0),
        total(total),
        progressColor(_progressColor),
        missingColor(_missingColor) {
    }

    void    increase(ulong n) { progress += n; }
    void increaseTot(ulong n) {    total += n; }

    void render(int terminalWidth);
};