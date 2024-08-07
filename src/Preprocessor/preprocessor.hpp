#pragma once
#include <string>
#include "Options.hpp"
#include "StructuredSource.hpp"
#include "ParsingResult.hpp"




namespace pre {
    StructuredSource loadSourceCode(Options &options);
    StructuredSource parseUnprocessedCode(std::string filePath, Options &options);

    ParsingResult     parseComment(std::string b, ulong index, ulong DEBUG_curLine);
    ParsingResult  parseStrLiteral(std::string b, ulong index, ulong DEBUG_curLine);
    ParsingResult parseCharLiteral(std::string b, ulong index, ulong DEBUG_curLine);
}