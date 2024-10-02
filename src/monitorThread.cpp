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




static void printStatusUI(std::string &fullCommand, ulong loop, const int progressBarWidth, bool _isComplete) {
    consoleLock.lock();

    // Adjust position and print command
    cout << "\033[s";             // Save cursor position
    cout << std::string(8, '\n'); // Push entire output 8 lines up (make space for the status UI)
    cout << "\033[999;999H";      // Move cursor to bottom-left corner
    cout << "\033[8A";            // Move cursor 8 lines up
    if(_isComplete) cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << " completed successfully.";
    else            cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << std::string("     ").replace(1 + abs((loop / 2) % 6 - 3), 1, 1, '-');



    // Print the status of each phase, in order
    phaseDataArrayLock.lock();
    for(ulong i = 0; i < phaseDataArray.size(); ++i) {
        const bool isPhaseComplete = phaseDataArray[i].timeEnd->load() > 0;
        const DynamicProgressBar *bar = phaseDataArray[i].totalProgress;

        cout
            << (isPhaseComplete ? ansi::bold_bright_green : ansi::bold_bright_black)
            << "\n    " << std::right << std::setw(maxPhaseNameLen) << phaseIdTotring((PhaseID)i) << " │ " << ansi::reset;

        if(isPhaseComplete) {
            cout
                << bar->max.load() << " steps"
                << " | "
                << utils::formatMilliseconds(phaseDataArray[i].timeEnd->load() - phaseDataArray[i].timeStart->load()) << " time elapsed"
            ;
        }
        else {
            // bar->render(progressBarWidth - 3 - 9); //FIXME USE THIS VERSION
            bar->render(progressBarWidth - 3 - 9 - 20);
            cout
                << "| "
                << std::left << std::setw(9) << utils::formatMilliseconds(utils::getEpochMs() - phaseDataArray[i].timeStart->load())
                << " "
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




    // Restore cursor position
    cout << "\033[u";

    consoleLock.unlock();
}








void startMonitorThread(std::string fullCommand){
    ulong interval = 100;

    // Set thread name
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
        static const int nameWidth = 20;  //! 20 is the maximum width of the phase name
        progressBarWidth = utils::getConsoleWidth();
        if(progressBarWidth == -1) progressBarWidth = nameWidth + 16; //! Totally arbitrary value
        progressBarWidth -= nameWidth;


        // Print status UI
        printStatusUI(fullCommand, loop, progressBarWidth, delayedIsCompleted);


        // Limit output refresh rate to 10fps
        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
        ++loop;

    } while(!delayedIsCompleted);
}