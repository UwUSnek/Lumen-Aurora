#pragma once
#include <string>
#include "Options.hpp"
#include "StructuredSource.hpp"
#include "ParsingResult.hpp"




namespace pre {
    StructuredSource loadSourceCode(Options &options);
    void loadSourceCode_loop(std::string filePath, Options& options, StructuredSource &output);
    void mergeSourceElements(StructuredSource &output, StructuredSource &r);

    std::pair<ulong, ulong> countWhitespaceCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    std::pair<ulong, ulong> countCommentCharacters(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);

    ParsingResult   parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ParsingResult  parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
    ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
}