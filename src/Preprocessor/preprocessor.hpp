#pragma once
#include <string>
#include "Options.hpp"
#include "ParsingResult.hpp"
#include "Preprocessor/CleanupPhase/LineReference.hpp"




namespace pre {
    std::pair<std::string, LineReference> loadSourceCode(Options &options);
    // void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output);
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r);
}