#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizedSource.hpp"




namespace cmp {
    void startTokenizationPhase(pre::SegmentedCleanSource *b, TokenizedSource *r);

    ulong                             countWhitespace(pre::SegmentedCleanSource *b, ulong index);
    std::optional<std::string> parseAlphanumericToken(pre::SegmentedCleanSource *b, ulong index);
    std::optional<std::string>     parseSymbolicToken(pre::SegmentedCleanSource *b, ulong index);


    bool   isWhitespaceChar(std::optional<char> const &c);
    bool isAlphanumericChar(std::optional<char> const &c);
    bool     isSymbolicChar(std::optional<char> const &c);
}