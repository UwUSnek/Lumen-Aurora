#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace misc {
    ulong measureLjt(pre::AnnotatedSource<false> &b, ulong index);
    ulong measureLct(pre::AnnotatedSource<false> &b, ulong index);
}