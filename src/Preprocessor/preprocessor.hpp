#pragma once
#include <string>
#include "Options.hpp"
#include "ParsingResult.hpp"




namespace pre {
    IntermediateCodeFormat loadSourceCode(Options &options);
    void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output);
    void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r);
}