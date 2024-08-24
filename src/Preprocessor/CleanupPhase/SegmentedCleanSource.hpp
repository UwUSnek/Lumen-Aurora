#pragma once
#include <string>
#include <tuple>
#include <vector>








namespace pre {
    /**
     * @brief A structure that contains informations about a single character
     */
    struct CleanSourceData {
        // CleanSourceType type;  // The type of the segment the character is part of
        ulong i;               // The original index of the character in the str buffer
        ulong l;               // The original line number of the character in the str buffer
        ulong f;               // The index of the original file of the character in the str buffer

        CleanSourceData(ulong _i, ulong _l, ulong _f) :
            i(_i),
            l(_l),
            f(_f) {
        }
    };









    /**
     * @brief This structure contain source code that has gone through the cleanup phase.
     *      It retains informations about any removed portion of code.
     *      If the original file is available, it allows every character of the code to be traced back to its original position.
     */
    struct SegmentedCleanSource {
        std::string str;                    // The clean string value
        std::vector<CleanSourceData> og;    // Informations about each character

        std::string toString();
    };
}