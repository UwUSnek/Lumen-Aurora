#include <algorithm>
#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <filesystem>
#include "monitorThread.hpp"
#include "Main/ALC.hpp"
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
        bar->render((int)(-3 /*Separator*/ + progressBarWidth - 2 /*Separator*/ - timeElapsedStrLen - 4 /*right margin*/));
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


static void printStatusUI(const std::string &fullCommand, ulong loop, const int progressBarWidth, const bool _isComplete) {
    bool hasError = exitMainRequest.load();
    cout++;


    // Adjust position and clear the console
    cout << std::format(
        "\033[s"             // Save current cursor position
        "\033[J"             // Clear console from current character to last line
        "{}"
        "\033[999;999H"      // Move cursor to bottom-left corner
        "\033[8A",           // Move cursor 8 lines up (make space for the status UI)
        std::string(8, '\n')
    );


    // Print the command
    if(_isComplete) {
        cout << std::format(
            "\n{}{}{} completed {}.",
            hasError ? ansi::bold_bright_red : ansi::bold_bright_green,
            fullCommand,
            ansi::reset,
            hasError ? "with errors" : "successfully"
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
    }


    // Print the status of each phase, in order
    {
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
                ansi::reset, fs::canonical(cmd::options->outputFile).string(), ansi::bold_bright_green
            )
        );
    }
    else {
        cout << std::format(
            "\n"
            "\n    {}t: {}{}/{}  |  {}f: {}{}  |  {}m: {}{}"
            "\n",
            ansi::bold_bright_green, ansi::reset, activeThreads.load(), totalThreads.load(),
            ansi::bold_bright_green, ansi::reset, totalFiles   .load(),
            ansi::bold_bright_green, ansi::reset, totalModules .load()
        );
    }


    // Restore cursor position and unlock output
    cout << "\033[u";
    cout--;
}








void startMonitorThread(const std::string fullCommand){ //NOSONAR
    const ulong interval = 100;

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


        // Print status UI
        if(cmd::options->printStatus) {
            printStatusUI(fullCommand, loop, progressBarWidth, delayedIsCompleted);
        }


        // Limit output refresh rate to 10fps
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        ++loop;

    } while(!delayedIsCompleted);
}