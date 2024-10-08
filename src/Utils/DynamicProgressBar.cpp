#include "ALC.hpp"
#include <iomanip>

#include "Utils/utils.hpp"
#include "Utils/ansi.hpp"
#include "DynamicProgressBar.hpp"








/**
 * @brief Renders the progress bar to stdout.
 *      This function is NOT thread safe. Use a mutex to ensure other threads don't print during rendering.
 * @param consoleWidth The number of spaces dedicated to the progress bar.
 *      Values lower than 0 are considered 0.
 */
void DynamicProgressBar::render(int consoleWidth) const {
    ulong _progress = progress.load();
    ulong _max = max.load();


    // Calculate actual progress bar width
    static const int valueWidth = 4; //! e.g. 430k
    consoleWidth -= (1 + valueWidth + 1 + valueWidth + 1);
    if(consoleWidth < 0) consoleWidth = 0;


    // Calculate the normalized progress. Goes from 0 to 1 instead of from 0 to total
    float normProgress = (float)_progress / (float)_max;
    // Calculate the amount of normalized progress each 1-character step is worth (depends on the width of the progress bar)
    float stepSize = 1.0f / (float)consoleWidth;


    // For each step
    for(float i = 0; i < 0.9999f; i += stepSize) {
        cout << (i < normProgress ? progressColor : missingColor) << "━";
    }


    // Reset colors and print value
    cout << ansi::reset <<
        " " <<
        std::right << std::setw(valueWidth) << utils::shortenInteger(_progress) <<
        "/" <<
        std::left << std::setw(valueWidth) << utils::shortenInteger(_max) <<
        " "
    ;
}