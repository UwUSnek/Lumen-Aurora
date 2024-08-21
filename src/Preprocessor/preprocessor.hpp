#pragma once
#include <string>
#include "Options.hpp"
#include "ParsingResult.hpp"
#include "Preprocessor/CleanupPhase/LineReference.hpp"




namespace pre {
    extern std::vector<std::string> sourceFilePaths;


    std::pair<std::string, LineReference> loadSourceCode(Options &options);
    std::pair<std::string, LineReference> loadSourceCode(std::string b, std::string filePath);


    // void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output);
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r);
}