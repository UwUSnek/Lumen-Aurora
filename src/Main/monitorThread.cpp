#include <algorithm>
#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <filesystem>
#include "monitorThread.hpp"
#include "Main/ALC.hpp"
#include "Main/errors.hpp"
#include "Utils/ansi.hpp"
#include "Command/command.hpp"
#include "Utils/console.hpp"
#include "Utils/format.hpp"

namespace fs = std::filesystem;
using namespace console;
static const uint timeElapsedStrLen = 12;











static int const maxPhaseNameLen = [] {
    #define X(e) #e,
    static std::vector<std::string> const names = { LIST_PHASE_ID };
    #undef X
    return (int)std::ranges::max_element(names, {}, std::mem_fn(&std::string::length))->length();
}();








static void renderProgressBar(const ulong i, const ulong progressBarWidth) {
    const bool isPhaseComplete = phaseDataArray[i].timeEnd  ->load() > 0;
    const bool isPhaseActive   = phaseDataArray[i].timeStart->load() > 0;
    const auto bar = phaseDataArray[i].totalProgress;

    cout << std::format(
        "{}"
        "\n    {:<{}} │ ",
        isPhaseComplete ? ansi::bold_bright_green : ansi::bold_bright_black,
        phaseIdTotring((PhaseID)i), maxPhaseNameLen
    );
    if(isPhaseComplete) {
        cout << std::format(
            "{}{:<{}} {}│{} {} steps", // MM:ss.mmm
            ansi::reset,
            format::milliseconds(phaseDataArray[i].timeEnd->load() - phaseDataArray[i].timeStart->load(), true), timeElapsedStrLen,
            ansi::bright_black, ansi::reset,
            format::amount(bar->max.load())
            //FIXME subtract pipe waiting times from this
        );
    }
    else {
        bar->render(-3 /*Separator*/ + (int)progressBarWidth - 2 /*Separator*/ - (int)timeElapsedStrLen - 4 /*right margin*/);
        cout << std::format(
            "{}│{} {:<{}}", // MM:ss.mmm
            ansi::bright_black, ansi::reset,
            format::milliseconds(isPhaseActive ? utils::getEpochMs() - phaseDataArray[i].timeStart->load() : 0, true), timeElapsedStrLen
            //FIXME subtract pipe waiting times from this
        );
    }
}


//TODO add something to Lumen that can replace the ANSI escape sequences.
//TODO ^ more readable code, easier to use. escape sequences will still be available.








static void printFileListUI() {
    //TODO list of file/modules. idk based on what
}








static void printStatusUI(const std::string &fullCommand, ulong loop, const ulong progressBarWidth, const bool _isComplete, const bool hasError) {

    // Print the command
    if(_isComplete) {
        cout << std::format(
            "\nTask completed {}."
            "\n{}{}{} ",
            hasError ? "with errors" : "successfully",
            hasError ? ansi::bold_bright_red : ansi::bold_bright_green, fullCommand, ansi::reset
        );
    }
    else {
        const long loadingWidth = 6;
        const long loadingPos = (long)loop / 2 % ((loadingWidth - 1) * 2);  // 0 to 9
        const long loadingPosWrapped = loadingPos < (loadingWidth - 1) ? loadingPos : ((loadingWidth - 1) * 2) - loadingPos;
        const char *loadingStr = loadingPosWrapped == 0 ? "╸" : (loadingPosWrapped == (loadingWidth - 1) ? "╺" : (loadingPos < (loadingWidth - 1) ? "╼" : "╾"));
        cout << ansi::reset << "\n" << "Building " << ansi::bright_black;
        for(long i = 0; i < loadingWidth; i++) {
            if(i == loadingPosWrapped) cout << ansi::white << loadingStr << ansi::bright_black;
            else cout << "─";
        }
        cout << "\n" << ansi::reset << ansi::bold_white << fullCommand;
    }


    // Print the status of each phase, in order
    {
        cout << "\n";
        std::scoped_lock lock(phaseDataArrayLock);
        for(ulong i = 0; i < phaseDataArray.size(); ++i) {
            renderProgressBar(i, progressBarWidth);
        }
    }


    // Print info line
    if(_isComplete) {
        cout << std::format(
            "\n"
            "\n    {}{}",
            hasError ? ansi::bold_bright_red : ansi::bold_bright_green,
            hasError ? "Errors were detected. Skipping file output." : std::format(
                "Output written to \"{}{}{}\"."
                "\n",
                ansi::reset, fs::canonical(cmd::options.outputFile).string(), ansi::bold_bright_green
            )
        );
    }
    else {
        cout << std::format(
            "\n"
            "\n    {}threads: {}{}/{}  |  {}files read: {}{}  |  {}modules loaded: {}{}"
            "\n",
            ansi::bold_bright_green, ansi::reset, activeThreads.load(), totalThreads.load(),
            ansi::bold_bright_green, ansi::reset, totalFiles   .load(),
            ansi::bold_bright_green, ansi::reset, totalModules .load()
        );
    }
}









void renderFrame(const std::string &fullCommand, ulong loop, const ulong progressBarWidth, const bool _isComplete) {
    const auto errorMsg = utils::getErrorMessage();
    cout++;


    // Set up render area
    // (Move cursor to top-left corner)
    // (Clear console from current character to last line)
    cout << "\033[1;999H";
    cout << "\033[J";

    // Move cursor to bottom-left corner
    // Print status UI
    cout << "\033[999;999H";
    if(cmd::options.printStatus) {
        printStatusUI(fullCommand, loop, progressBarWidth, _isComplete, errorMsg.has_value());
    }

    // Move cursor to top-left corner
    // Print errors if present. Print list of active files/modules otherwise
    cout << "\033[1;999H";
    if(errorMsg.has_value()) cout << errorMsg.value() << ansi::reset;
    else printFileListUI();


    cout--;
}








void startMonitorThread(const std::string fullCommand){ //NOSONAR

    // Set thread name and type
    threadType = ThreadType::MONITOR;
    pthread_setname_np(pthread_self(), "Monitor");


    ulong loop = 0;
    int progressBarWidth;
    bool delayedIsCompleted;
    do {
        delayedIsCompleted = isComplete.load(std::memory_order_acquire); //! Delay completion detection by 1 iteration to allow the last frame to be fully printed before returning


        // Collect local progresses and update the progress bar
        {
            std::scoped_lock lock(subphaseDataArrayLock, phaseDataArrayLock);
            for(auto const& e : subphaseDataArray) {
                phaseDataArray[(ulong)e.phaseId].totalProgress->increase(e.localProgress->exchange(0));
            }
        }


        // Calculate progress bar width
        progressBarWidth = utils::getConsoleWidth() - maxPhaseNameLen - 4 /* Indentation */;
        if(progressBarWidth == -1) progressBarWidth = 16; //! 16 is an arbitrary value


        // Render frame - limit output refresh rate to 20fps
        const ulong interval = 1000UL / 20UL;
        renderFrame(fullCommand, loop, (ulong)progressBarWidth, delayedIsCompleted);
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));


        // Update loop counter
        ++loop;

    } while(!delayedIsCompleted);
}