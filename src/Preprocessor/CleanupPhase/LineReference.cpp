#include "LineReference.hpp"
#include "Utils/utils.hpp"




namespace pre {
    std::string LineReferencedSource::toString() {
        std::string r;
        for(ulong i = 0; i < ref.size(); ++i) {
            if(i) r += "\n";
            r += "F" + std::to_string(ref[i].OG_fileIndex) + " L" + std::to_string(ref[i].OG_lineNum) + " │ ";
            for(ulong j = ref[i].start; j < ref[i].start + ref[i].len; ++j) {
                r += utils::formatChar(str[j], true);
            }
        }
        r += "\n";
        return r;
    }
}