#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <cstring>
#include <thread>
#include <cmath>
namespace fs = std::filesystem;


#include "ALC.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"
#include "Command/command.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "Compiler/compiler.hpp"
#include "Command/info.hpp"








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




void printStatusUI(std::string &fullCommand, ulong loop, const int progressBarWidth, bool _isComplete) {
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
            bar->render(progressBarWidth - 3 - 9);
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








void writeOutputFile(std::string &code) {
    // Create directories
    fs::create_directories(fs::path(cmd::options.outputFile).parent_path());


    // Write the file and print an error if it cannot be created
    std::ofstream f(cmd::options.outputFile);
    if(f.is_open()) {
        f << code;
        f.close();
    }
    else {
        utils::printErrorGeneric(
            ErrorCode::ERROR_OUTPUT_CANNOT_CREATE,
            "Could not write output file \"" + cmd::options.outputFile + "\".\n" +
            "Output path was interpreted as: \"" + ansi::white + fs::canonical(cmd::options.outputFile).string() + ansi::reset + "\".\n"
        );
    }
}







int main(int argc, char* argv[]){
    threadType = ThreadType::MAIN;


    // Recreate full command
    std::string fullCommand;
    for(int i = 0; i < argc; ++i) {
        if(i) fullCommand += " ";
        fullCommand += std::string(argv[i]);
    }


    // Parse command line options
    cmd::parseOptions(argc, argv, fullCommand);
    if(cmd::options.isHelp) {
        consoleLock.lock();
        cout << cmd::getHelpMessage();
        consoleLock.unlock();
        exit(0);
    }
    if(cmd::options.isVersion) {
        consoleLock.lock();
        cout << cmd::getVersionMessage();
        consoleLock.unlock();
        exit(0);
    }

    bool compileModule  = cmd::options.outputType == 'x' || cmd::options.outputType == 'm';
    bool compileExec    = cmd::options.outputType == 'x';






    // Start monitor thread
    std::thread monitorThread(startMonitorThread, fullCommand);




    // Preprocessing
    std::ifstream f(cmd::options.sourceFile);
    std::string s = utils::readFile(f);
    f.close();
    totalFiles.fetch_add(1);
    pre::SegmentedCleanSource *preprocessedSourceCode = pre::loadSourceCode(&s, cmd::options.sourceFile);


    if(compileModule) {
        // Compilation
        cmp::compilePreprocessedSourceCode(preprocessedSourceCode);


        // Optimization
        //TODO actually optimize the code
    }


    if(compileExec) {
        // Conversion to C and gcc compilation
        //TODO actually convert the code
    }






    //TODO only print additional timings and info if requested through the command
    //TODO cross out skipped phases when using -e, -p or --o-none
    // Wait for subphase threads to complete
    while(activeThreads.load()) {
        int exitCode = exitMainRequest.load();
        if(exitCode) {
            std::exit(exitCode);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }



    // Write output file
    if(compileExec) {
        //TODO write exec
    }
    else if(compileModule) {
        //TODO write module
    }
    else {
        preprocessedSourceCode->str.awaitClose();
        preprocessedSourceCode->str.sReallocLock.lock();
        writeOutputFile(*preprocessedSourceCode->str.cpp());
        preprocessedSourceCode->str.sReallocLock.unlock();
    }



    // Join monitor thread
    isComplete.store(true);
    monitorThread.join();
}