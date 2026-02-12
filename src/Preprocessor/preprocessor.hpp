#pragma once
#include <functional>
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"





//FIXME make this a command line option
// The size of the character buffers (one for each phase)
#define PREPROCESSOR_BUFFER_SIZE_LARGE_MB 500UL
#define PREPROCESSOR_BUFFER_SIZE_SMALL_MB 5000UL
#define PREPROCESSOR_BUFFER_SIZE_LARGE ((PREPROCESSOR_BUFFER_SIZE_LARGE_MB * 1000UL * 1000UL) / sizeof(pre::CleanSourceElm))
#define PREPROCESSOR_BUFFER_SIZE_SMALL ((PREPROCESSOR_BUFFER_SIZE_SMALL_MB * 1000UL * 1000UL) / sizeof(pre::CleanSourceElm))


namespace pre {

    ptr<SegmentedCleanSource<false>> loadSourceCode_loop(
        const std::string &s,
        const std::string &filePath,
        const std::function<bool()> &awaitTask = [](){ return true; }
    );

    ptr<SegmentedCleanSource<false>> loadSourceCode(
        const std::string &s,
        const std::string &filePath
    );
}