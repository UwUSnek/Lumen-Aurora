#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    enum class TextLiteralType {
        STRING,
        CHAR
    };

    ptr<TokenValue> parseTextLiteral(ptr<pre::SegmentedCleanSource> b, ulong index, ulong *rawLiteralLen, TextLiteralType literalType);
}