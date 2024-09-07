#include "ALC.hpp"
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>
#include <cstring>
#include <thread>
#include <cmath>
namespace fs = std::filesystem;


#include "Command/command.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"












void printStatusUI(std::string &fullCommand, ulong loop, const int progressBarWidth, bool _isComplete) {
    consoleLock.lock();

    // Adjust position and print command
    cout << "\033[s";             // Save cursor position
    cout << std::string(8, '\n'); // Push entire output 8 lines up (make space for the status UI) //FIXME understand why this is not working
    cout << "\033[999;999H";      // Move cursor to bottom-left corner
    cout << "\033[8A";            // Move cursor 8 lines up
    if(_isComplete) cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << " completed successfully.";
    else            cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::reset << std::string("     ").replace(1 + abs((loop / 2) % 6 - 3), 1, 1, '-');



    // Print the status of each phase, in order
    phaseDataArrayLock.lock(); //FIXME replace pre::totalProgress with progressbar saved in the Phase array
    for(ulong i = 0; i < phaseDataArray.size(); ++i) {
        //FIXME replace pre::totalProgress with progressbar saved in the Phase array
        const bool isPhaseComplete = phaseDataArray[i].totalProgress->isComplete();
        const DynamicProgressBar *bar = phaseDataArray[i].totalProgress;

        cout << (isPhaseComplete ? ansi::bold_bright_green : ansi::bold_bright_black) << "\n    " << phaseIdTotring((PhaseID)i) << " │ " << ansi::reset;
        if(isPhaseComplete) {
            // cout << ansi::reset << std::fixed << std::setprecision(3) <<  timePre.count() << " seconds, " << bar->total.load() << " steps."; //FIXME write timings
            cout << ansi::reset << std::fixed << std::setprecision(3) <<  0 << " seconds, " << bar->max.load() << " steps.";
        }
        else {
            // phaseDataArray[i].totalProgress->render(progressBarWidth - 3 - 5); //FIXME write timings
           bar->render(progressBarWidth);
        }
    }
    phaseDataArrayLock.unlock(); //FIXME replace pre::totalProgress with progressbar saved in the Phase array


    // // Print preprocessor status
    // cout << (pre::totalProgress.isComplete() ? ansi::bold_bright_green : ansi::bold_bright_black) << "\n\033[K    Preprocessing │ " << ansi::reset;
    // if(pre::totalProgress.isComplete()) cout << ansi::reset << std::fixed << std::setprecision(3) <<  timePre.count() << " seconds, " << pre::totalProgress.total.load() << " steps.";
    // else pre::totalProgress.render(progressBarWidth);


    // // Print compilation status
    // cout << ansi::bold_bright_black << "\n\033[K    Compilation   │ " << ansi::reset;
    // if(true) cout << ansi::reset << std::fixed << std::setprecision(3) << timeComp.count() << " seconds."; //TODO
    // else 0; //TODO

    // // Print optimization status
    // cout << ansi::bold_bright_black << "\n\033[K    Optimization  │ " << ansi::reset;
    // if(true) cout << ansi::reset << std::fixed << std::setprecision(3) <<  timeOpt.count() << " seconds."; //TODO
    // else 0; //TODO


    // // Print conversion status
    // cout << ansi::bold_bright_black << "\n\033[K    Conversion    │ " << ansi::reset;
    // if(true) cout << ansi::reset << std::fixed << std::setprecision(3) << timeConv.count() << " seconds."; //TODO
    // else 0; //TODO


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
            // if(e.phaseId == PhaseID::PREPROCESSING) pre::totalProgress.increase(e.localProgress->load());
        }
        subphaseDataArrayLock.unlock();
        // localProgressArrayLock.lock();
        // for(ulong i = 0; i < localProgressArray.size(); ++i) {
            // pre::totalProgress.increase(localProgressArray[i]->exchange(0));
        // }
        // localProgressArrayLock.unlock();



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


    // // Print final update before joining main thread
    // printStatusUI(fullCommand, loop, progressBarWidth, true);
}









//TODO real time progress bares for each phase, all visible at once
//TODO real time progress bares for each phase, all visible at once
//TODO real time progress bares for each phase, all visible at once
//TODO real time progress bares for each phase, all visible at once
//TODO real time progress bares for each phase, all visible at once
//TODO real time progress bares for each phase, all visible at once
//TODO real time progress bares for each phase, all visible at once

//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
//TODO 100% VALUE CHANGES WHEN A NEW FILE OR SECTION IS DISCOVERED. PROGRESS BAR SHOWS THE TOTAL PROGRESS OF ALL THREADS
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







//TODO run all passes concurrently. make the next pass wait for the previous one when it reached it and make them read from streams
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
        cmd::printHelp();
        exit(0);
    }
    if(cmd::options.isVersion) {
        cmd::printVersion();
        exit(0);
    }

    // bool preprocessOnly = cmd::options.outputType == 'p';
    bool compileModule  = cmd::options.outputType == 'x' || cmd::options.outputType == 'm';
    bool compileExec    = cmd::options.outputType == 'x';






    // Start monitor thread
    std::thread monitorThread(startMonitorThread, fullCommand);



    //TODO add an option to suppress fancy real-time output

    //FIXME write info and estimated percentage progress instead of the time spent after the name of the phase.
    //FIXME color them green and replace them with the time after completing it.

    //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
    // Preprocessing
    // timeStartPre = std::chrono::high_resolution_clock::now();
    std::string s = utils::readAndCheckFile(cmd::options.sourceFile);
    totalFiles.fetch_add(1);
    pre::SegmentedCleanSource &sourceCode = pre::loadSourceCode(&s, cmd::options.sourceFile);
    // timePre = std::chrono::high_resolution_clock::now() - timeStartPre;




    // if(cmd::options.outputType == 'p') {
        // writeOutputFile(*sourceCode.str.cpp());
        //TODO write source informations if requested
    // }
    if(compileModule) {
        //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
        // Compilation
        // timeStartComp = std::chrono::high_resolution_clock::now();
        //TODO actually compile the code
        // timeComp = std::chrono::high_resolution_clock::now() - timeStartComp;




        //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
        // Optimization
        // timeStartOpt = std::chrono::high_resolution_clock::now();
        //TODO actually optimize the code
        // timeOpt = std::chrono::high_resolution_clock::now() - timeStartOpt;
    }



    if(compileExec) {
        //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
        // Conversion to C and gcc compilation
        // timeStartConv = std::chrono::high_resolution_clock::now();
        //TODO actually convert the code
        // timeConv = std::chrono::high_resolution_clock::now() - timeStartConv;

        // writeOutputFile(sourceCode.str); //TODO use the output from the conversion phase
    }




    //TODO only print additional timings and info if requested through the command
    //TODO cross out skipped phases when using -e, -p or --o-none
    // Join subphase threads
    while(activeThreads.load()) {
        int exitCode = utils::exitMainRequest.load();
        if(exitCode) {
            std::exit(exitCode);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    // threadsLock.lock();
    // for(auto &t : threads) t.join();
    // threadsLock.unlock();



    // Write output file
    if(compileExec) {
        //TODO write exec
    }
    else if(compileModule) {
        //TODO write module
    }
    else {
        sourceCode.str.sReallocLock.lock();
        writeOutputFile(*sourceCode.str.cpp());
        sourceCode.str.sReallocLock.unlock();
    }



    // Join monitor thread
    isComplete.store(true);
    monitorThread.join();
}