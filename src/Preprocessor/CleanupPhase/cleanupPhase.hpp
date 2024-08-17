#pragma once
#include <string>
#include "Preprocessor/IntermediateCodeFormat.hpp"
#include "WhitespaceInfo.hpp"
#include "Preprocessor/ParsingResult.hpp"




namespace pre {
    ulong checkLct(std::string b, ulong index);
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    WhitespaceInfo       countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);


    ParsingResult  parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);


    IntermediateCodeFormat startCleanupPhase(std::string rawCode, std::string filePath);
}