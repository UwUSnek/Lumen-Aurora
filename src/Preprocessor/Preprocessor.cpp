#include <iostream>
#include <fstream>
#include <filesystem>
#include <cerrno>
#include <cstring>

#include "Preprocessor.hpp"


//TODO move to utils file
namespace ansi {
    std::string fgBlack   = "\033[30m";    std::string bgBlack   = "\033[40m";
    std::string fgRed     = "\033[31m";    std::string bgRed     = "\033[41m";
    std::string fgGreen   = "\033[32m";    std::string bgGreen   = "\033[42m";
    std::string fgYellow  = "\033[33m";    std::string bgYellow  = "\033[43m";
    std::string fgBlue    = "\033[34m";    std::string bgBlue    = "\033[44m";
    std::string fgMagenta = "\033[35m";    std::string bgMagenta = "\033[45m";
    std::string fgCyan    = "\033[36m";    std::string bgCyan    = "\033[46m";
    std::string fgWhite   = "\033[37m";    std::string bgWhite   = "\033[47m";

    std::string bold       = "\033[1m";    std::string boldOff      = "\033[21m";
    std::string underline  = "\033[4m";    std::string underlineOff = "\033[24m";
    std::string inverse    = "\033[7m";    std::string inverseOff   = "\033[27m";

    std::string reset      = "\033[0m";

    //! Bold often uses a brighter shade of the same color
    //! Inverse swaps foreground and background colors
}



namespace pre {
    //TODO move to utils file
    static void printError(std::string message) {
        std::cout << message;
    }

    static std::string readAndCheckFile(std::string fileName) {
        std::ifstream f(fileName);
        if(!f) {
            std::cerr << ansi::fgRed << ansi::bold << "Preprocessor error:" << ansi::reset << "\n";
            std::cerr << "    Could not open file \"" << fileName << "\": " << std::strerror(errno) << ".\n";
            std::cerr << "    Current working directory is: " << std::filesystem::current_path() << ".\n";
            exit(1);
        }

        std::string l, r;
        while(f >> l) r += l + "\n";
        std::cout << r;
        return r;
    }




    std::vector<std::string> loadSourceFiles(Options& options) {
        std::vector<std::string> r;

        for(int i = 0; i < options.sourceFiles.size(); ++i) {
            r.push_back(readAndCheckFile(options.sourceFiles[i]));
        }
        return r;
    }




    std::string processIncludes() {

    }
}