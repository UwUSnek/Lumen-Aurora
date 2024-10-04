#pragma once
#include <optional>
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"




namespace cmp {
    TokenValue*                     parseStrLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    TokenValue*                    parseCharLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    std::optional<std::string> decodeEscapeSequence(pre::SegmentedCleanSource *b, ulong index, ulong *rawEscapeLen);
}