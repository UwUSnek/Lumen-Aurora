#include "LineReference.hpp"
#include "Utils/utils.hpp"



namespace pre {
    std::string lineReferencedDataToString(std::pair<std::string, LineReference> b) {
        std::string r;

        ulong lastLineStart = 0;
        ulong i = 0, curLine = 0;
        while(i < b.first.length()) {
            while(i == b.second[curLine]) {
                if(curLine) r += "\n";
                ++curLine;
                r += std::to_string(curLine) + " | ";
            }
            r += utils::formatChar(b.first[i], true);
            ++i;
        }

        r += "\n";
        return r;
    }
}