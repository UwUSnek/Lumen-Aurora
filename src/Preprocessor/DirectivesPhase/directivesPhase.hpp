#pragma once
#include "Preprocessor/IntermediateCodeFormat.hpp"



namespace pre {
    IntermediateCodeFormat startDirectivesPhase(IntermediateCodeFormat cleanCode, std::string DEBUG_filePath);

    void processInclude(IntermediateCodeFormat &r, IntermediateCodeFormat &b, ulong i, std::smatch &match, std::smatch &OG_match, std::string DBG_filePath);
}