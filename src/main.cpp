#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>
#include <filesystem>

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
void writeOutputFile(std::string code) {
    std::filesystem::create_directories(cmd::options.outputFile);
    std::ofstream f(cmd::options.outputFile);
    if(f.is_open()) {
        f << code;
        f.close();
    }
    else {
        //TODO print error if output file cannot be opened
    }
}







//TODO run all passes concurrently. make the next pass wait for the previous one when it reached it and make them read from streams
int main(int argc, char* argv[]){
    cmd::parseOptions(argc, argv);
    if(cmd::options.isHelp) {
        cmd::printHelp();
        exit(0);
    }
    if(cmd::options.isVersion) {
        cmd::printVersion();
        exit(0);
    }


    // Phase clocks and durations (they are set right before starting each phase and the duration calculated after it ends)
    std::chrono::system_clock::time_point timeStartPre;   std::chrono::duration<double> timePre;
    std::chrono::system_clock::time_point timeStartComp;  std::chrono::duration<double> timeComp;
    std::chrono::system_clock::time_point timeStartOpt;   std::chrono::duration<double> timeOpt;
    std::chrono::system_clock::time_point timeStartConv;  std::chrono::duration<double> timeConv;




    //TODO add an option to suppress fancy real-time output

    //FIXME write info and estimated percentage progress instead of the time spent after the name of the phase.
    //FIXME color them green and replace them with the time after completing it.

    //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
    // Preprocessing
    timeStartPre = std::chrono::high_resolution_clock::now();
    pre::SegmentedCleanSource sourceCode = pre::loadSourceCode(cmd::options.sourceFile);
    timePre = std::chrono::high_resolution_clock::now() - timeStartPre;




    if(cmd::options.outputType == 'p') {
        writeOutputFile(sourceCode.str);
        //TODO write source informations if requested
    }
    else {
        //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
        // Compilation
        timeStartComp = std::chrono::high_resolution_clock::now();
        //TODO actually compile the code
        timeComp = std::chrono::high_resolution_clock::now() - timeStartComp;




        //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
        // Optimization
        timeStartOpt = std::chrono::high_resolution_clock::now();
        //TODO actually optimize the code
        timeOpt = std::chrono::high_resolution_clock::now() - timeStartOpt;




        if(cmd::options.outputType == 'm') {
            // writeOutputFile(sourceCode.str); //TODO write precompiled module
            //TODO write source informations if requested
        }
        else {
            //FIXME write progress bar and update output in real time. only show errors/success when they actually happen and clear the progress
            // Conversion
            timeStartConv = std::chrono::high_resolution_clock::now();
            //TODO actually convert the code
            timeConv = std::chrono::high_resolution_clock::now() - timeStartConv;
        }
    }




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