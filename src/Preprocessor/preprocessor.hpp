#pragma once
#include <functional>
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {

    ptr<SegmentedCleanSource> loadSourceCode_loop(
        const std::string &s,
        const std::string &filePath,
        const std::function<bool()> &awaitTask = [](){ return true; }
    );

    ptr<SegmentedCleanSource> loadSourceCode(
        const std::string &s,
        const std::string &filePath
    );
}