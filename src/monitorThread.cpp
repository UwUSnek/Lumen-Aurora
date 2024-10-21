#include <vector>
#include <filesystem>
namespace fs = std::filesystem;

#include "monitorThread.hpp"
#include "ALC.hpp"
#include "Utils/ansi.hpp"
#include "Command/command.hpp"










static ulong const maxPhaseNameLen = [] {
    #define X(e) #e,
    static std::vector<std::string> const names = { LIST_PHASE_ID };
    #undef X
    return std::max_element(names.begin(), names.end(),
        [](std::string const &a, std::string const &b) {
            return a.length() < b.length();
        }
    )->length();
}();

//TODO add something to Lumen that can replace the ANSI escape sequences.
//TODO ^ more readable code, easier to use. escape sequences will still be available.


static void printStatusUI(std::string &fullCommand, ulong loop, const int progressBarWidth, bool _isComplete) {
    cout++;

    // Adjust position, clear the console and print the command
    cout << "\033[s";             // Save current cursor position
    cout << "\033[J";             // Clear console from current character to last line
    cout << std::string(8, '\n');
    cout << "\033[999;999H";      // Move cursor to bottom-left corner
    cout << "\033[8A";            // Move cursor 8 lines up (make space for the status UI)
    if(_isComplete) cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << " completed successfully.";
    // else            cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << std::string("     ").replace(1 + abs((loop / 2) % 6 - 3), 1, 1, '-');
    else            cout << ansi::reset << "\n" << std::string("     ").replace(1 + abs((loop / 2) % 6 - 3), 1, 1, '-');



    // Print the status of each phase, in order
    phaseDataArrayLock.lock();
    for(ulong i = 0; i < phaseDataArray.size(); ++i) {
        const bool isPhaseComplete = phaseDataArray[i].timeEnd  ->load() > 0;
        const bool isPhaseActive   = phaseDataArray[i].timeStart->load() > 0;
        const DynamicProgressBar *bar = phaseDataArray[i].totalProgress;

        cout
            << (isPhaseComplete ? ansi::bold_bright_green : ansi::bold_bright_black)
            << "\n    " << std::right << std::setw(maxPhaseNameLen) << phaseIdTotring((PhaseID)i) << " │ ";

        if(isPhaseComplete) {
            cout
                << ansi::reset
                << std::left << std::setw((9 /*MM:ss.mmm*/) + sizeof(" time elapsed") - 1)
                << (utils::formatMilliseconds(phaseDataArray[i].timeEnd->load() - phaseDataArray[i].timeStart->load()) + " time elapsed")
                << ansi::bright_black << " │ " << ansi::reset
                << bar->max.load() << " steps"
            ;
        }
        else {
            static const uint timeElapsedStrLen = 9;
            bar->render(-(3 /*Separator*/) + progressBarWidth - (2 /*Separator*/) - timeElapsedStrLen - (4 /*right margin*/));
            cout
                << ansi::bright_black << "│ " << ansi::reset
                << std::left << std::setw((9 /*MM:ss.mmm*/))
                << utils::formatMilliseconds(isPhaseActive ? utils::getEpochMs() - phaseDataArray[i].timeStart->load() : 0)
            ;
        }
    }
    phaseDataArrayLock.unlock();




    // Print info line
    if(_isComplete) cout << ansi::bold_bright_green << "\n\n    Output written to \"" << ansi::reset << fs::canonical(cmd::options.outputFile).string() << ansi::bold_bright_green << "\".\n";
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








void startMonitorThread(std::string fullCommand){
    ulong interval = 100;

    // Set thread name and type
    threadType = ThreadType::MONITOR;
    pthread_setname_np(pthread_self(), "Monitor");


    ulong loop = 0;
    int progressBarWidth;
    bool delayedIsCompleted;
    do {
        delayedIsCompleted = isComplete.load(); //! Delay completion detection by 1 iteration to allow the last frame to be fully printed before returning


        // Collect local progresses and update the progress bar
        subphaseDataArrayLock.lock();
        for(auto const& e : subphaseDataArray) {
            phaseDataArrayLock.lock();
            phaseDataArray[e.phaseId].totalProgress->increase(e.localProgress->exchange(0));
            phaseDataArrayLock.unlock();
        }
        subphaseDataArrayLock.unlock();


        // Calculate progress bar width
        progressBarWidth = utils::getConsoleWidth() - maxPhaseNameLen - (4 /* Indentation */);
        if(progressBarWidth == -1) progressBarWidth = 16; //! 16 is an arbitrary value


        // Print status UI
        if(cmd::options.printStatus) {
            printStatusUI(fullCommand, loop, progressBarWidth, delayedIsCompleted);
        }


        // Limit output refresh rate to 10fps
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        ++loop;

    } while(!delayedIsCompleted);
}