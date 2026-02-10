#include "Main/ALC.hpp"
#include <iomanip>

#include "Utils/console.hpp"
#include "Utils/format.hpp"
#include "Utils/ansi.hpp"
#include "DynamicProgressBar.hpp"








/**
 * @brief Renders the progress bar to stdout.
 *      This function is NOT thread safe. Use a mutex to ensure other threads don't print during rendering.
 * @param consoleWidth The number of spaces dedicated to the progress bar.
 *      Values lower than 0 are considered 0.
 */
void DynamicProgressBar::render(int consoleWidth) const {
    using namespace console;
    ulong _progress = progress.load();
    ulong _max = max.load();


    // Calculate actual progress bar width
    static const int valueWidth = 4; //! e.g. 430k
    consoleWidth -= (1 + valueWidth + 1 + valueWidth + 1);
    if(consoleWidth < 0) consoleWidth = 0;


    // Calculate filled and missing width of the line in characters
    auto filledWidth = std::clamp((int)((float)_progress / (float)_max * (float)consoleWidth), 0, consoleWidth);
    int missingWidth = consoleWidth - filledWidth;


    // Print line and progress values
    cout << std::format(
        "{}{:━<{}}"
        "{}{:━<{}}"
        " {}{:>{}}/{:<{}} ",
        progressColor, "", filledWidth,
        missingColor,  "", missingWidth,
        ansi::reset,
        format::shortenInteger(_progress), valueWidth,
        format::shortenInteger(_max),      valueWidth
    );
}