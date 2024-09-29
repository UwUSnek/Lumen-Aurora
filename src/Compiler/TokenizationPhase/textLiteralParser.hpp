#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizedSource.hpp"




namespace cmp {
    TokenValue*                     parseStrLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    TokenValue*                    parseCharLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    std::optional<std::string> decodeEscapeSequence(pre::SegmentedCleanSource *b, ulong index, ulong *rawEscapeLen);
}