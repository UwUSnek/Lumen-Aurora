#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

#include "Command/command.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"











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








//TODO run all passes concurrently. make the next pass wait for the previous one when it reached it and make them read from streams
int main(int argc, char* argv[]){
    cmd::Options options = cmd::parseOptions(argc, argv);
    if(options.isHelp) {
        cmd::printHelp();
        exit(0);
    }
    if(options.isVersion) {
        cmd::printVersion();
        exit(0);
    }




    //TODO add an option to suppress fancy real-time output

    //FIXME write info and estimated percentage progress instead of the time spent after the name of the phase.
    //FIXME color them green and replace them with the time after completing it.

    //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
    // Preprocessing
    auto timeStartPre = std::chrono::high_resolution_clock::now();
    pre::SegmentedCleanSource sourceCode = pre::loadSourceCode(options);
    // std::cout << sourceCode.toStringVerbose() << "\n";
    std::cout << sourceCode.toString() << "\n";
    std::chrono::duration<double> timePre = std::chrono::high_resolution_clock::now() - timeStartPre;




    //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
    // Compilation
    auto timeStartComp = std::chrono::high_resolution_clock::now();
    //TODO
    std::chrono::duration<double> timeComp = std::chrono::high_resolution_clock::now() - timeStartComp;




    //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
    // Optimization
    auto timeStartOpt = std::chrono::high_resolution_clock::now();
    //TODO actually optimize the code
    std::chrono::duration<double> timeOpt = std::chrono::high_resolution_clock::now() - timeStartOpt;




    //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
    // Conversion
    auto timeStartConv = std::chrono::high_resolution_clock::now();
    //TODO actually optimize the code
    std::chrono::duration<double> timeConv = std::chrono::high_resolution_clock::now() - timeStartConv;




    //TODO only print additional timings and info if requested through the command
    //TODO cross out skipped phases when using -e, -p or --o-none
    // Successful command output
    std::string fullCommand; for(int i = 0; i < argc; ++i) fullCommand += std::string(argv[i]) + ' ';
    std::cout << ansi::bold_green << fullCommand << ansi::reset << "completed successfully.";
    std::cout << ansi::bold_green << "\n    Preprocessing │ " << ansi::reset << std::fixed << std::setprecision(3) << timePre.count()  << " seconds.";
    std::cout << ansi::bold_green << "\n    Compilation   │ " << ansi::reset << std::fixed << std::setprecision(3) << timeComp.count() << " seconds.";
    std::cout << ansi::bold_green << "\n    Optimization  │ " << ansi::reset << std::fixed << std::setprecision(3) << timeOpt.count()  << " seconds.";
    std::cout << ansi::bold_green << "\n    Conversion    │ " << ansi::reset << std::fixed << std::setprecision(3) << timeConv.count() << " seconds.";
    // std::cout << "\n    Output written to \"\""; //TODO
    std::cout << "\n\n";
}