#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace misc {
    ulong countWhitespace(pre::SegmentedCleanSource<false> &b, ulong index);
    bool  isWhitespaceChar(const char &c);
}