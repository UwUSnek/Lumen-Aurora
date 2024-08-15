#pragma once
#include <string>
#include "Options.hpp"
#include "StructuredSource.hpp"
#include "Parser/ParsingResult.hpp"
#include "Parser/WhitespaceInfo.hpp"




namespace pre {
    StructuredSource loadSourceCode(Options &options);
    void loadSourceCode_loop(std::string filePath, Options& options, StructuredSource &output);
    void mergeSourceElements(StructuredSource &output, StructuredSource &r);

    WhitespaceInfo countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);

    ParsingResult   parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ParsingResult  parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
}