#pragma once
#include <string>
#include <vector>
#include <atomic>
#include <mutex>

#include "Command/Options.hpp"
#include "ParsingResult.hpp"
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"
#include "Utils/DynamicProgressBar.hpp"




namespace pre {
    // extern DynamicProgressBar               totalProgress; //FIXME MOVE TO PHASE ARRAY

    // extern thread_local ulong threadId;

    // void initPhaseThread();
    // void stopPhaseThread();



    extern std::vector<std::string> sourceFilePaths;
    extern std::mutex sourceFilePathsLock;


    SegmentedCleanSource& loadSourceCode(std::string const *s, std::string const &filePath);
    // SegmentedCleanSource& loadSourceCode(std::string &filePath);


    // void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output);
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r);
}