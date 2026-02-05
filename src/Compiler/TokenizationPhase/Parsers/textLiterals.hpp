#pragma once
#include <optional>
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"




namespace cmp {
    enum class DelimitedLiteralType {
        STRING,
        CHAR
    };

    TokenValue*               parseDelimitedLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen, DelimitedLiteralType literalType);
    std::optional<std::string> decodeEscapeSequence(pre::SegmentedCleanSource *b, ulong index, ulong *rawEscapeLen,  DelimitedLiteralType literalType);
}