#pragma once
#include <functional>
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {

    ptr<SegmentedCleanSource<true>> loadSourceCode_loop(
        const std::string &s,
        const std::string &filePath,
        const std::function<bool()> &awaitTask = [](){ return true; }
    );

    ptr<SegmentedCleanSource<true>> loadSourceCode(
        const std::string &s,
        const std::string &filePath
    );
}