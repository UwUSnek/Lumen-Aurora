#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void generateMetadata(const std::string &rawCode, ptr<SegmentedCleanSource<true>> r, ulong DBG_filePathIndex);
}