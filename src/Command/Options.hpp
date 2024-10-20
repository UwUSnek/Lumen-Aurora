#pragma once
#include <vector>
#include <string>


namespace cmd {
    struct Options {
        // Input and output file paths
        std::string sourceFile;
        std::string outputFile;

        // Include and Import paths
        std::vector<std::string> includePaths;
        std::vector<std::string> importPaths;

        // Command overrides
        bool isHelp = false;
        bool isVersion = false;

        // 'p' = preprocess source file
        // 'm' = precompile module
        // 'x' = compile executable
        char outputType = '\0';

        //TODO implement this option
        // 'l' = linux
        // 'w' = windows
        char targetPlatform = '\0';



        // Output control
        bool printColor = true;
        bool printDisplay = true; //TODO implement this option
        bool printErrors = true;  //TODO implement this option
        bool printStatus = true;
        bool silent = false;      //TODO implement this option


        //TODO add optimization options

        Options(){}
    };
}



