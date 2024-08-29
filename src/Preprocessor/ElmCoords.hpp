#pragma once
#include <string>


namespace pre {
    struct SegmentedCleanSource;
}
struct ElmCoords {
    std::string filePath;
    ulong lineNum;
    ulong start;
    ulong end;

    ElmCoords() :
        filePath(""),
        lineNum(0),
        start(0),
        end(0) {
    }

    ElmCoords(std::string _fileName, ulong _lineNum, ulong _start, ulong _end) :
        filePath(_fileName),
        lineNum(_lineNum),
        start(_start),
        end(_end) {
    }

    ElmCoords(pre::SegmentedCleanSource source, ulong _start, ulong _end);
};