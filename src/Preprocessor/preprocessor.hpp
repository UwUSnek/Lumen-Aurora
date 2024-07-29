#pragma once
#include "Options.hpp"

namespace pre {
    std::string loadSourceFile(Options& options);
    std::string processDirectives(std::string s);
}