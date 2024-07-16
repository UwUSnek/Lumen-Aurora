#pragma once
#include "Options.hpp"

namespace pre {
    std::string loadSourceFile(Options& options);
    std::string processIncludes(std::string sourceFile);
}