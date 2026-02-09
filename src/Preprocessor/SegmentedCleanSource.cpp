#include "SegmentedCleanSource.hpp"
#include "Utils/format.hpp"




std::string pre::SegmentedCleanSource::toString() {
    std::stringstream r;

    for(ulong i = 0; i < str.length(); ++i) {
        char c = *str[i];
        r << format::whitespace(c, meta[i]->c, true);
        if(c == '\n') r << "\n";
    }

    return r.str();
}