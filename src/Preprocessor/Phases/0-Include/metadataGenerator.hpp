#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void generateMetadata(std::string rawCode, ptr<SegmentedCleanSource> r, ulong DBG_filePathIndex);
}