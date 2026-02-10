#pragma once
#include <optional>
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    std::optional<std::string> parseAlphanumericToken(ptr<pre::SegmentedCleanSource> b, ulong index);
    bool isAlphanumericChar(std::optional<char> const &c);
    bool isAlphanumericCharFirst(std::optional<char> const &c);


    std::optional<std::string> parseSymbolicToken(ptr<pre::SegmentedCleanSource> b, ulong index);
    bool isSymbolicChar(std::optional<char> const &c);
    bool isCharReserved(std::optional<char> const &c);
}