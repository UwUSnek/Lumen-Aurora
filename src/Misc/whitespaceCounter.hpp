#pragma once
#include "Utils/Containers/StringPipe.hpp"




namespace misc {
    ulong countWhitespace(StringPipe &b, ulong index);
    bool  isWhitespaceChar(std::optional<char> const &c);
}