#pragma once
#include <string>
#include <atomic>








/**
 * @brief A thread-safe progress bar that measures progress in steps and can change the maximum steps dynamically.
 *      It features a render function that draws a progress bar to stdout.
 */
struct DynamicProgressBar {
    std::atomic<ulong> progress;     // The current progress in units
    std::atomic<ulong> max;          // The total units of progress required to reach 100%

    std::string progressColor;
    std::string missingColor;




public:
    DynamicProgressBar() = delete;
    DynamicProgressBar(ulong _max, std::string _progressColor, std::string _missingColor) :
        progress(0),
        max(_max),
        progressColor(_progressColor),
        missingColor(_missingColor) {
    }


    void    increase(ulong n) { progress.fetch_add(n); }
    void increaseMax(ulong n) {      max.fetch_add(n); }
    void decreaseMax(ulong n) {      max.fetch_sub(n); }

    void render(int terminalWidth) const;
    // bool isComplete() const { return progress.load() >= max.load(); }
};