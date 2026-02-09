#include <algorithm>
#include <atomic>
#include <functional>
#include <mutex>
#include <vector>
#include <filesystem>
#include "monitorThread.hpp"
#include "ALC.hpp"
#include "Utils/ansi.hpp"
#include "Command/command.hpp"
#include "Utils/format.hpp"

namespace fs = std::filesystem;












static int const maxPhaseNameLen = [] {
    #define X(e) #e,
    static std::vector<std::string> const names = { LIST_PHASE_ID };
    #undef X
    return (int)std::ranges::max_element(names, {}, std::mem_fn(&std::string::length))->length();
}();




static void renderProgressBar(const ulong i, const ulong progressBarWidth) {
    const bool isPhaseComplete = phaseDataArray[i].timeEnd  ->load() > 0;
    const bool isPhaseActive   = phaseDataArray[i].timeStart->load() > 0;
    const DynamicProgressBar *bar = phaseDataArray[i].totalProgress;

    cout
        << (isPhaseComplete ? ansi::bold_bright_green : ansi::bold_bright_black)
        << "\n    " << std::left << std::setw(maxPhaseNameLen) << phaseIdTotring((PhaseID)i) << " │ ";

    if(isPhaseComplete) {
        cout
            << ansi::reset
            << std::left << std::setw(9 /*MM:ss.mmm*/ + sizeof(" time elapsed") - 1) //FIXME subtract pipe waiting times from this
            << (format::milliseconds(phaseDataArray[i].timeEnd->load() - phaseDataArray[i].timeStart->load()) + " time elapsed") //FIXME subtract pipe waiting times from this
            << ansi::bright_black << " │ " << ansi::reset
            << format::amount(bar->max.load()) << " steps"
        ;
    }
    else {
        static const uint timeElapsedStrLen = 9;
        bar->render((int)(-3 /*Separator*/ + progressBarWidth - 2 /*Separator*/ - timeElapsedStrLen - 4 /*right margin*/));
        cout
            << ansi::bright_black << "│ " << ansi::reset
            << std::left << std::setw(9 /*MM:ss.mmm*/)
            << format::milliseconds(isPhaseActive ? utils::getEpochMs() - phaseDataArray[i].timeStart->load() : 0) //FIXME subtract pipe waiting times from this
        ;
    }
}


//TODO add something to Lumen that can replace the ANSI escape sequences.
//TODO ^ more readable code, easier to use. escape sequences will still be available.


static void printStatusUI(const std::string &fullCommand, ulong loop, const int progressBarWidth, const bool _isComplete) {
    cout++;

    // Adjust position, clear the console and print the command
    cout << "\033[s";             // Save current cursor position
    cout << "\033[J";             // Clear console from current character to last line
    cout << std::string(8, '\n');
    cout << "\033[999;999H";      // Move cursor to bottom-left corner
    cout << "\033[8A";            // Move cursor 8 lines up (make space for the status UI)
    if(_isComplete) {
        if(exitMainRequest.load()) {
            cout << ansi::bold_bright_red << "\n" << fullCommand << ansi::reset << " completed with errors.";
        }
        else {
            cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << " completed successfully.";
        }
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
        if(exitMainRequest.load()) {
            cout << ansi::bold_bright_red << "\n\n    Errors were detected. Skipping file output.";
        }
        else {
            cout << ansi::bold_bright_green << "\n\n    Output written to \"" << ansi::reset << fs::canonical(cmd::options->outputFile).string() << ansi::bold_bright_green << "\".\n";
        }
    }
    else {
        cout << "\n\n    ";
        cout << ansi::bold_bright_green << "t: " << ansi::reset << activeThreads.load() << "/" << totalThreads.load() << "  |  ";
        cout << ansi::bold_bright_green << "f: " << ansi::reset << totalFiles.load() << "  |  ";
        cout << ansi::bold_bright_green << "m: " << ansi::reset << totalModules.load();
        cout << "\n";
    }


    // Restore cursor position and unlock output
    cout << "\033[u";
    cout--;
}








void startMonitorThread(const std::string &fullCommand){
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
                phaseDataArray[e.phaseId].totalProgress->increase(e.localProgress->exchange(0));
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