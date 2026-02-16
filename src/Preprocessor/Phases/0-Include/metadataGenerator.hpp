#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"




namespace pre {
    void generateMetadata(const std::string &rawCode, ptr<AnnotatedSource<false>> r, ulong DBG_filePathIndex);
}