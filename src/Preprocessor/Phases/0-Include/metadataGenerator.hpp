#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void generateMetadata(std::string rawCode, SegmentedCleanSource* r, ulong DBG_filePathIndex);
}