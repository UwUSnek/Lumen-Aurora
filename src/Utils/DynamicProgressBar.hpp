#pragma once
#include <string>
#include <atomic>








/**
 * @brief A thread-safe progress bar that measures progress in steps and can change the maximum steps dynamically.
 *      It features a render function that draws a progress bar to stdout.
 */
struct DynamicProgressBar {
private:
    std::atomic<ulong> progress = 0;    // The current progress in units
    std::atomic<ulong> max;             // The total units of progress required to reach 100%
    std::atomic<bool> error = false;    // Whether the process using represented by this progress generated errors




public:
    DynamicProgressBar() = delete;
    explicit DynamicProgressBar(ulong _max) :
        max(_max) {
    }


    // Progress
    void    increase(ulong n) { progress.fetch_add(n); }
    void increaseMax(ulong n) {      max.fetch_add(n); }
    void decreaseMax(ulong n) {      max.fetch_sub(n); }
    ulong getProgress() const { return progress.load(); }
    ulong getMax() const { return max.load(); }

    // Errorrs
    void flagError() { error.store(true); }
    bool hasError() const { return error.load(); }

    // Rendering
    void render(int terminalWidth, const std::string &progressColor, const std::string &missingColor) const;
    // bool isComplete() const { return progress.load() >= max.load(); }
};