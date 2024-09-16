#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace cmp {
    std::optional<std::string>      parseStrLiteral(pre::SegmentedCleanSource *b, ulong index);
    std::optional<std::string>     parseCharLiteral(pre::SegmentedCleanSource *b, ulong index);
    std::optional<std::string> decodeEscapeSequence(pre::SegmentedCleanSource *b, ulong index, char literalType, ulong *rawEscapeLen);
}