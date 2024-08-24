#pragma once
#include <string>
#include "Options.hpp"
#include "ParsingResult.hpp"
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"




namespace pre {
    extern std::vector<std::string> sourceFilePaths;


    SegmentedCleanSource loadSourceCode(Options &options);
    SegmentedCleanSource loadSourceCode(std::string b, std::string filePath);


    // void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output);
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r);
}