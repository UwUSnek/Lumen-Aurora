#pragma once
#include <optional>
#include "Preprocessor/SegmentedCleanSource.hpp"




namespace cmp {
    std::optional<std::string> parseAlphanumericToken(pre::SegmentedCleanSource *b, ulong index);
    bool isAlphanumericChar(std::optional<char> const &c);


    std::optional<std::string>     parseSymbolicToken(pre::SegmentedCleanSource *b, ulong index);
    bool     isSymbolicChar(std::optional<char> const &c);
}