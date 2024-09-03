#include <iostream>
#include <iomanip>

#include "Utils/utils.hpp"
#include "DynamicProgressBar.hpp"








void DynamicProgressBar::render(int consoleWidth) {
    ulong _progress = progress.load();
    ulong _total = total.load();


    // Calculate actual progress bar width
    static const int valueWidth = 4; //! e.g. 430k
    consoleWidth -= (1 + valueWidth + 1 + valueWidth + 1);


    // Calculate the normalized progress. Goes from 0 to 1 instead of from 0 to total
    float normProgress = (float)_progress / (float)_total;
    // Calculate the amount of normalized progress each 1-character step is worth (depends on the width of the progress bar)
    float stepSize = 1.0f / (float)consoleWidth;


    // For each step
    for(float i = 0; i < 0.9999f; i += stepSize) {
        std::cout << (i < normProgress ? progressColor : missingColor) << "━";
    }


    // Reset colors and print value
    std::cout << ansi::reset <<
        " " <<
        std::right << std::setw(valueWidth) << utils::shortenInteger(_progress) <<
        "/" <<
        std::left << std::setw(valueWidth) << utils::shortenInteger(_total) <<
        " "
    ;
}