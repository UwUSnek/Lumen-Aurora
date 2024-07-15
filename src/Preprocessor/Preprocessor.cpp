#include <iostream>
#include "Preprocessor.hpp"


namespace pre {
    std::string mergeSourceFiles(Options& options) {
        for(int i = 0; i < options.sourceFiles.size(); ++i) {
            std::cout << options.sourceFiles[i] + "\n";
        }
        return std::string("a");
    }
}