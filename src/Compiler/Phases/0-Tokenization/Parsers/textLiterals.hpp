#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"




namespace cmp {
    enum class TextLiteralType {
        STRING,
        CHAR
    };

    TokenValue* parseTextLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen, TextLiteralType literalType);
}