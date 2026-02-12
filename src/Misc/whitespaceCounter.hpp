#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace misc {
    ulong countWhitespace(pre::AnnotatedSource<false> &b, ulong index);
    bool  isWhitespaceChar(const char &c);
}