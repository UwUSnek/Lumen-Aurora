#pragma once
#include <string>
#include "Preprocessor/CleanupPhase/LineReference.hpp"



namespace pre {
    std::pair<std::string, LineReference> startDirectivesPhase(std::pair<std::string, LineReference> &b, std::string DEBUG_filePath);

    // void processInclude(IntermediateCodeFormat &r, IntermediateCodeFormat &b, ulong i, std::smatch &match, std::smatch &OG_match, std::string DBG_filePath);
}