#pragma once
#include <string>
#include "SegmentedCleanSource.hpp"




namespace pre {
    SegmentedCleanSource startCleanupPhase(SegmentedCleanSource &b);

    // WhitespaceInfo countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ulong       measureComment(std::string &b, ulong index);
    ulong       saveLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r);

    // ulong      parseStrLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r);
    // ulong     parseCharLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r);
    // ulong decodeEscapeSequence(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r, CleanSourceType literalType);
}