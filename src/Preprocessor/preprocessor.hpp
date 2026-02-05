#pragma once
#include <functional>
#include <string>

#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    SegmentedCleanSource* loadSourceCode_loop(const std::string *s, const std::string &filePath, const std::function<void()> &awaitTask = [](){}); //NOSONAR empty lambda
    SegmentedCleanSource*      loadSourceCode(const std::string *s, const std::string &filePath);
}