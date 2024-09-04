#include <iomanip>
#include "SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"




namespace pre {
    //FIXME pass multi-byte characters whole to allow the console to actually display them
    std::string SegmentedCleanSource::toString() {
        std::stringstream r;

        for(ulong i = 0; i < str.length(); ++i) {
            char c = *str[i];
            r << utils::formatChar(c, true);
            if(c == '\n') r << "\n";
        }

        return r.str();
    }




    //FIXME pass multi-byte characters whole to allow the console to actually display them
    std::string SegmentedCleanSource::toStringVerbose() {
        std::stringstream r;

        ulong lineStart = 0;
        for(ulong i = 0; i < str.length(); ++i) {
            char c = *str[i];
            r << utils::formatChar(c, true);

            if(c == '\n' || i == str.length() - 1) {
                r << "\n";
                for(ulong j = lineStart; j <= i; ++j) {
                    switch(meta[j].type) {
                        case CleanSourceType::MISC:   { r << 'm'; continue; }
                        case CleanSourceType::STRING: { r << 's'; continue; }
                        case CleanSourceType::CHAR:   { r << 'c'; continue; }
                    }
                }
                r << "\n";
                r << "\n";
                lineStart = i + 1;
            }
        }

        return r.str();
    }
}