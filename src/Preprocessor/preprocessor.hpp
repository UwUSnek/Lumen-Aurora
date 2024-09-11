#pragma once
#include <string>
#include <vector>
#include <atomic>
#include <mutex>

#include "Command/Options.hpp"
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"
#include "Utils/DynamicProgressBar.hpp"




namespace pre {
    SegmentedCleanSource& loadSourceCode_loop(std::string const *s, std::string const &filePath);
    SegmentedCleanSource& loadSourceCode(std::string const *s, std::string const &filePath);
}