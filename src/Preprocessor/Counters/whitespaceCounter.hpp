#pragma once
#include "Preprocessor/AnnotatedSource.hpp"




namespace misc {
    ulong measureWhitespace(pre::AnnotatedSource<false> &b, ulong index);
    bool  isWhitespaceChar(const char &c);
}