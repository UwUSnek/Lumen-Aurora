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
#include "monitorThread.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"








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

    // Set thread name and type
    threadType = ThreadType::MAIN;
    pthread_setname_np(pthread_self(), "Main Thread");


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


    // Initialize phase data
    initPhaseData();




    // Preprocessing
    std::ifstream f(cmd::options.sourceFile);
    std::string s = utils::readFile(f);
    f.close();
    totalFiles.fetch_add(1);
    pre::SegmentedCleanSource *preprocessedSourceCode = pre::loadSourceCode(&s, cmd::options.sourceFile);
    cmp::SourceTree *precompiledModule = nullptr;
    // pre::SegmentedCleanSource *convertedCode     = nullptr; //TODO


    if(compileModule) {
        // Compilation
        precompiledModule = cmp::compilePreprocessedSourceCode(preprocessedSourceCode);


        // Optimization
        //TODO actually optimize the code
    }


    if(compileExec) {
        // Conversion to C and gcc compilation
        //TODO actually convert the code
    }






    //TODO only print additional timings and info if requested through the command
    //TODO cross out skipped phases when using -e, -p or --o-none



    // Write output file
    if(compileExec) {
        //TODO write exec
    }
    else if(compileModule) {
        precompiledModule->awaitClose(mainCheckErrors);
        //TODO write module
    }
    else {
        preprocessedSourceCode->str.awaitClose(mainCheckErrors);
        preprocessedSourceCode->meta.awaitClose(mainCheckErrors);
        preprocessedSourceCode->str.sReallocLock.lock();
        writeOutputFile(*preprocessedSourceCode->str.cpp());
        preprocessedSourceCode->str.sReallocLock.unlock();
    }



    // Join monitor thread
    isComplete.store(true);
    monitorThread.join();
}