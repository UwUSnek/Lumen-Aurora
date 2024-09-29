#include "Compiler/TokenizedSource.hpp"




namespace cmp {
    TokenValue* parseNumericalLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    TokenValue* parseTextNumericalLiteral(pre::SegmentedCleanSource *b, ulong index, ulong *rawLiteralLen);
    double strToDbl(std::string const &s, uint base);
}