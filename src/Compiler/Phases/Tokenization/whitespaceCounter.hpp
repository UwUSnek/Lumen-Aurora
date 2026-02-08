#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace cmp {
    ulong countWhitespace(pre::SegmentedCleanSource *b, ulong index);
    bool  isWhitespaceChar(std::optional<char> const &c);
}