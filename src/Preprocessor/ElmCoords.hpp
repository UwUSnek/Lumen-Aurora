#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"




struct ElmCoords {
    ulong filePathIndex;
    ulong lineNum;
    ulong start;
    ulong end;

    ElmCoords() :
        filePathIndex(-1),
        lineNum(0),
        start(0),
        end(0) {
    }

    ElmCoords(ulong _filePathIndex, ulong _lineNum, ulong _start, ulong _end) :
        filePathIndex(_filePathIndex),
        lineNum(_lineNum),
        start(_start),
        end(_end) {
    }

    ElmCoords(pre::SegmentedCleanSource *source, ulong _start, ulong _end);
    ElmCoords(cmp::TokenizedSource      *source, ulong _start, ulong _end);
};