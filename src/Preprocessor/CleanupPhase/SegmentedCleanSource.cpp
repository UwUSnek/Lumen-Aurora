#include <iomanip>
#include "SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"



namespace pre {
    std::string SegmentedCleanSource::toString() {
        std::stringstream r;

        for(ulong i = 0; i < str.length(); ++i) {
            char c = str[i];
            r << utils::formatChar(c, true);
            if(c == '\n') r << "\n";
        }

        return r.str();
    }
}