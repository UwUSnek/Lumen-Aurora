#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace cmp {
    std::optional<std::string>      parseStrLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    std::optional<std::string>     parseCharLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    std::optional<std::string> decodeEscapeSequence(pre::SegmentedCleanSource *b, ulong index, ulong *rawEscapeLen);
}