#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <chrono>

#include "Options.hpp"
#include "Preprocessor/preprocessor.hpp"




Options parseOptions(int argc, char* argv[]){
    Options r;
    for(int i = 1; i < argc; ++i) {
        r.sourceFile = std::string(argv[i]);
    }
    return r;
}




//TODO run all passes concurrently. make the next pass wait for the previous one when it reached it and make them read from streams
int main(int argc, char* argv[]){
    Options options = parseOptions(argc, argv);


    // Preprocessing
    auto timeStartPre = std::chrono::high_resolution_clock::now();
    pre::IntermediateCodeFormat sourceCode = pre::loadSourceCode(options);
    std::cout << sourceCode.toString();
    std::chrono::duration<double> timePre = std::chrono::high_resolution_clock::now() - timeStartPre;


    // Compilation
    auto timeStartComp = std::chrono::high_resolution_clock::now();
    //TODO
    std::chrono::duration<double> timeComp = std::chrono::high_resolution_clock::now() - timeStartComp;


    // Optimization
    auto timeStartOpt = std::chrono::high_resolution_clock::now();
    //TODO actually optimize the code
    std::chrono::duration<double> timeOpt = std::chrono::high_resolution_clock::now() - timeStartOpt;


    // Successful command output
    std::string fullCommand; for(int i = 0; i < argc; ++i) fullCommand += std::string(argv[i]) + ' ';
    std::cout << ansi::bold_green << fullCommand << ansi::reset << "completed successfully.";
    std::cout << ansi::bold_green << "\n    Preprocessing │ " << ansi::reset << std::fixed << std::setprecision(3) << timePre.count()  << " seconds.";
    std::cout << ansi::bold_green << "\n    Compilation   │ " << ansi::reset << std::fixed << std::setprecision(3) << timeComp.count() << " seconds.";
    std::cout << ansi::bold_green << "\n    Optimization  │ " << ansi::reset << std::fixed << std::setprecision(3) << timeOpt.count() << " seconds.";
    // std::cout << "\n    Output written to \"\""; //TODO
    std::cout << "\n\n";
}