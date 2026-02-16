#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    std::string parseAlphanumericToken(ptr<pre::AnnotatedSource<false>> b, ulong index);
    bool isAlphanumericChar(const char c);
    bool isAlphanumericCharFirst(const char c);


    std::string parseSymbolicIdentifier(ptr<pre::AnnotatedSource<false>> b, ulong index);
    std::string parseSymbolicKeyword(ptr<pre::AnnotatedSource<false>> b, ulong index);
    bool isSymbolicChar(const char c);
    bool isCharReserved(const char c);
}