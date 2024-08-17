#pragma once
#include <string>
#include "Options.hpp"
#include "StructuredSource.hpp"
#include "CleanupPhase/ParsingResult.hpp"
#include "CleanupPhase/cleanupPhase.hpp"




namespace pre {
    StructuredSource loadSourceCode(Options &options);
    void loadSourceCode_loop(std::string filePath, Options& options, StructuredSource &output);
    void mergeSourceElements(StructuredSource &output, StructuredSource &r);
}