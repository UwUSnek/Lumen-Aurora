#include <iomanip>
#include "SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"




std::string pre::SegmentedCleanSource::toString() {
    std::stringstream r;

    for(ulong i = 0; i < str.length(); ++i) {
        char c = *str[i];
        r << utils::formatChar(c, meta[i]->c, true);
        if(c == '\n') r << "\n";
    }

    return r.str();
}