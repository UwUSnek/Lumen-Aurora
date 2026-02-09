#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"




namespace cmp {
    TokenValue* parseNumericalLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    double strToDbl(const std::string &s, uint base);
    ulong  strToLng(const std::string &s, uint base);
}