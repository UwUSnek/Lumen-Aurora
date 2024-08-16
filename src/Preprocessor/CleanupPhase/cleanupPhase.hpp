#pragma once
#include <string>
#include "ICF_Clean.hpp"
#include "WhitespaceInfo.hpp"
#include "ParsingResult.hpp"




namespace pre {
    ulong checkLct(std::string b, ulong index);
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    WhitespaceInfo       countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);


    ParsingResult  parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);


    ICF_Clean startCleanupPhase(std::string rawCode, std::string filePath);
}