#pragma once
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    SegmentedCleanSource* loadSourceCode_loop(std::string const *s, std::string const &filePath);
    SegmentedCleanSource*      loadSourceCode(std::string const *s, std::string const &filePath);
}