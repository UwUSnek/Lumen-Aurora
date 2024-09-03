#include <iostream>
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

//FIXME detect all the files before processing them to make progress more reliable
// Becomes true after the output file gets written and closed
std::atomic<bool> isComplete(false);




void printStatusUI(std::string &fullCommand, ulong loop, const int progressBarWidth)
{
    // Print progress
    std::cout << "\033[s";             // Save cursor position
    std::cout << std::string(7, '\n'); // Push entire output 7 lines up (make space for the status UI)
    std::cout << "\033[999;999H";      // Move cursor to bottom-left corner
    std::cout << "\033[7A";            // Move cursor 7 lines up
    std::cout << ansi::bold_bright_green << "\n" << fullCommand << ansi::white << std::string("     ").replace(1 + abs((loop / 2) % 6 - 3), 1, 1, '-');
    std::cout << (pre::totalProgress.isComplete() ? ansi::bold_bright_green : ansi::bold_bright_black) << "\n    Preprocessing │ " << ansi::reset;
    pre::totalProgress.render(progressBarWidth);
    std::cout << ansi::bold_bright_black << "\n    Compilation   │ " << ansi::reset;
    std::cout << ansi::bold_bright_black << "\n    Optimization  │ " << ansi::reset;
    std::cout << ansi::bold_bright_black << "\n    Conversion    │ " << ansi::reset;
    std::cout << "\n\n";
    std::cout << "\033[u"; // Restore cursor position
}





void startMonitorThread(std::string fullCommand){
    ulong delay = 100;


    for(ulong loop = 0; !isComplete.load(); ++loop) {
        // if(loop > 1000 / delay) loop = 0;

        // Collect local progresses and update the progress bar
        for(ulong i = 0; i < pre::localProgress.size(); ++i) {
            pre::totalProgress.increase(pre::localProgress[i]->exchange(0));
        }


        // Calculate progress bar width
        static const int nameWidth = 20;  //! 20 is the maximum width of the phase name
        const int progressBarWidth = utils::getConsoleWidth() - nameWidth;

        // Print status UI
        printStatusUI(fullCommand, loop, progressBarWidth);

        // Limit output refresh rate to 10fps
        std::this_thread::sleep_for(std::chrono::milliseconds(delay));
    }
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
void writeOutputFile(std::string code) {
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


    // Phase clocks and durations (they are set right before starting each phase and the duration calculated after it ends)
    std::chrono::_V2::system_clock::time_point timeStartPre;   std::chrono::duration<double> timePre;
    std::chrono::_V2::system_clock::time_point timeStartComp;  std::chrono::duration<double> timeComp;
    std::chrono::_V2::system_clock::time_point timeStartOpt;   std::chrono::duration<double> timeOpt;
    std::chrono::_V2::system_clock::time_point timeStartConv;  std::chrono::duration<double> timeConv;






    // Start monitor thread
    std::thread monitorThread(startMonitorThread, fullCommand);



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
            // Conversion to C and gcc compilation
            timeStartConv = std::chrono::high_resolution_clock::now();
            //TODO actually convert the code
            timeConv = std::chrono::high_resolution_clock::now() - timeStartConv;

            // writeOutputFile(sourceCode.str); //TODO use the output from the conversion phase
        }
    }




    //TODO only print additional timings and info if requested through the command
    //TODO cross out skipped phases when using -e, -p or --o-none
    // Join monitor thread
    isComplete.store(true);
    monitorThread.join();

    // Successful command output
    std::cout << ansi::bold_bright_green << fullCommand << ansi::reset << " completed successfully.";
    std::cout << ansi::bold_bright_green << "\n    Preprocessing │ " << ansi::reset << std::fixed << std::setprecision(3) <<  timePre.count() << " seconds.";
    std::cout << ansi::bold_bright_green << "\n    Compilation   │ " << ansi::reset << std::fixed << std::setprecision(3) << timeComp.count() << " seconds.";
    std::cout << ansi::bold_bright_green << "\n    Optimization  │ " << ansi::reset << std::fixed << std::setprecision(3) <<  timeOpt.count() << " seconds.";
    std::cout << ansi::bold_bright_green << "\n    Conversion    │ " << ansi::reset << std::fixed << std::setprecision(3) << timeConv.count() << " seconds.";
    std::cout << ansi::bold_bright_green << "\n\n    Output written to \"" << ansi::reset << fs::canonical(cmd::options.outputFile).string() << ansi::bold_bright_green << "\".";
    std::cout << "\n\n";
}