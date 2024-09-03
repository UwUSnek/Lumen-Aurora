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
    extern DynamicProgressBar totalProgress;
    extern std::vector<std::atomic<ulong>*> localProgress;
    extern std::mutex localProgressLock;
    extern thread_local ulong threadId;

    void initPhaseThread();
    void increaseLocalProgress(ulong n);



    extern std::vector<std::string> sourceFilePaths;

    SegmentedCleanSource loadSourceCode(std::string s, std::string filePath);
    SegmentedCleanSource loadSourceCode(std::string filePath);


    // void loadSourceCode_loop(std::string filePath, Options& options, IntermediateCodeFormat &output);
    // void mergeSourceElements(IntermediateCodeFormat &output, IntermediateCodeFormat &r);
}