#pragma once
#include <string>
#include "SegmentedCleanSource.hpp"




namespace pre {
    ulong checkLct(std::string b, ulong index);
    SegmentedCleanSource startCleanupPhase(std::string b, ulong DBG_filePathIndex);

    // WhitespaceInfo countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    std::pair<ulong, ulong>       measureComment(std::string b, ulong index);
    std::pair<ulong, ulong>      parseStrLiteral(std::string b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource& r);
    std::pair<ulong, ulong>     parseCharLiteral(std::string b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource& r);
    std::pair<ulong, ulong> decodeEscapeSequence(std::string b, ulong index, ulong DBG_curLine, ulong DBG_filePathIndex, SegmentedCleanSource& r);
}