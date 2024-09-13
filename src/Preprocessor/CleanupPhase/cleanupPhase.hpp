#pragma once
#include <string>
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace pre {
    void startCleanupPhase(SegmentedCleanSource *b, SegmentedCleanSource *r);

    // WhitespaceInfo countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ulong measureComment(StringPipe &b, ulong index);
    ulong saveLiteral(SegmentedCleanSource *b, ulong index, SegmentedCleanSource *r);

    // ulong      parseStrLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r);
    // ulong     parseCharLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r);
    // ulong decodeEscapeSequence(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r, CleanSourceType literalType);
}