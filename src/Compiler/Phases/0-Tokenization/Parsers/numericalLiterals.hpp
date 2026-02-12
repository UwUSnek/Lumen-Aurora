#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




namespace cmp {
    ptr<TokenValue> parseNumericalLiteral(ptr<pre::SegmentedCleanSource<true>> b, ulong index, ulong *rawLiteralLen);
    double strToDbl(const std::string &s, uint base);
    ulong  strToLng(const std::string &s, uint base);
}