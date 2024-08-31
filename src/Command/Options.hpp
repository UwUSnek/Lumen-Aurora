#pragma once
#include <vector>
#include <string>


namespace cmd {
    struct Options {
        std::string sourceFile;
        std::string outputFile;

        bool isHelp = false;
        bool isVersion = false;

        // 'p' = preprocess source file
        // 'm' = precompile module
        // 'x' = compile executable
        char outputType = '0';

        // 'l' = linux
        // 'w' = windows
        char targetPlatform = '0';
    };
}



