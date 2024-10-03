#pragma once
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    SegmentedCleanSource* loadSourceCode_loop(std::string const *s, std::string const &filePath, void (*awaitTask)() = [](){});
    SegmentedCleanSource*      loadSourceCode(std::string const *s, std::string const &filePath);
}