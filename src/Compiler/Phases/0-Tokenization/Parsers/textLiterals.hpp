#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    enum class TextLiteralType {
        STRING,
        CHAR
    };

    ptr<TokenValue> parseTextLiteral(ptr<pre::AnnotatedSource<false>> b, ulong index, ulong *rawLiteralLen, TextLiteralType literalType);
}