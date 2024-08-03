#pragma once
#include "Options.hpp"
#include "StructuredSource.hpp"




namespace pre {
    std::string loadSourceFile(Options& options);
    StructuredSource processDirectives(std::string s);


    ulong computeCommentLen(std::string b, ulong index);
    ulong computeStrLiteralLen(std::string b, ulong index);
    ulong checkCharLiteral(std::string b, ulong index);
}