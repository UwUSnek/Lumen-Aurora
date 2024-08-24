#pragma once
#include <string>
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"



namespace pre {
    SegmentedCleanSource startIncludePhase(SegmentedCleanSource &b, ulong DBG_filePathIndex);

    void NewFunction(pre::SegmentedCleanSource &b, ulong &sgmi, pre::SegmentedCleanSource &r, ulong &OG_i);

    // void processInclude(IntermediateCodeFormat &r, IntermediateCodeFormat &b, ulong i, std::smatch &match, std::smatch &OG_match, std::string DBG_filePath);
}