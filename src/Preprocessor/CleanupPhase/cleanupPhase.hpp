#pragma once
#include <string>
#include "WhitespaceInfo.hpp"
#include "SegmentedCleanSource.hpp"




namespace pre {
    ulong checkLct(std::string b, ulong index);
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DBG_filePathIndex);
    // WhitespaceInfo       countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);


    // ParsingResult  parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    // ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);


    SegmentedCleanSource startCleanupPhase(std::string b, ulong DBG_filePathIndex);
}