#pragma once
#include <string>
#include "SegmentedCleanSource.hpp"




namespace pre {
    ulong checkLct(std::string b, ulong index);
    SegmentedCleanSource startCleanupPhase(SegmentedCleanSource &b);

    // WhitespaceInfo countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ulong       measureComment(std::string &b, ulong index);
    ulong      parseStrLiteral(SegmentedCleanSource &b, ulong index, SegmentedCleanSource &r);
    ulong     parseCharLiteral(SegmentedCleanSource &b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource &r);
    ulong decodeEscapeSequence(SegmentedCleanSource &b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource &r);
}