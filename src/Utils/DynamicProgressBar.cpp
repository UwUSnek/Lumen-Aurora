#include <iostream>
#include "Utils/ansi.hpp"
#include "DynamicProgressBar.hpp"




void DynamicProgressBar::render(int consoleWidth) {
    // The normalized progress. Goes from 0 to 1 instead of from 0 to total
    float normProgress = (float)progress / (float)total;

    // The amount of normalized progress each 1-character step is worth (depends on the width of the console)
    float stepSize = 1.0f / (float)consoleWidth;


    // For each step
    for(float i = 0; i < 0.9999f; i += stepSize) {
        std::cout << (i < normProgress ? progressColor : missingColor) << " ";
    }
    std::cout << ansi::reset;
}