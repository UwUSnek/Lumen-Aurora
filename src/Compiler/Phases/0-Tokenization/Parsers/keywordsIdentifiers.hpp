#pragma once
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    std::string parseAlphanumericToken(ptr<pre::SegmentedCleanSource<true>> b, ulong index);
    bool isAlphanumericChar(const char c);
    bool isAlphanumericCharFirst(const char c);


    std::string parseSymbolicIdentifier(ptr<pre::SegmentedCleanSource<true>> b, ulong index);
    std::string parseSymbolicKeyword(ptr<pre::SegmentedCleanSource<true>> b, ulong index);
    bool isSymbolicChar(const char c);
    bool isCharReserved(const char c);
}