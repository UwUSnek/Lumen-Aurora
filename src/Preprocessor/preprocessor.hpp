#pragma once
#include <string>
#include "Options.hpp"
#include "StructuredSource.hpp"
#include "ParsingResult.hpp"




namespace pre {
    StructuredSource loadSourceCode(Options &options);
    StructuredSource parseUnprocessedCode(std::string filePath, Options &options);

    ParsingResult parseComment(std::string b, ulong index);
    ParsingResult parseStrLiteral(std::string b, ulong index);
    ParsingResult parseCharLiteral(std::string b, ulong index);
}