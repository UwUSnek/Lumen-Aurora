#pragma once
#include <functional>
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    SegmentedCleanSource* loadSourceCode_loop(const std::string *s, const std::string &filePath, const std::function<void()> &awaitTask = [](){});
    SegmentedCleanSource*      loadSourceCode(const std::string *s, const std::string &filePath);
}