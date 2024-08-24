#pragma once
#include <string>
#include <tuple>
#include <vector>




namespace pre {
    struct SourceSegment {
        ulong OG_start, len;    // The start index and length of the segment, relative to the original file
        ulong OG_lineNum;       // The number of the line in which the segment starts, relative to the original file
        ulong OG_fileIndex;     // The index of the file path list in which the path to the original file is saved
        bool isRemoved;         // Whether the segment was removed during the cleanup phase

        SourceSegment(ulong _OG_start, ulong _len, ulong _OG_lineNum, ulong _OG_fileIndex, bool _isRemoved) :
            OG_start(_OG_start),
            len(_len),
            OG_lineNum(_OG_lineNum),
            OG_fileIndex(_OG_fileIndex),
            isRemoved(_isRemoved) {
        }
    };




    /**
     * @brief This structure contain source code that has gone through the cleanup phase.
     *      It retains informations about any removed portion of code.
     *      If the original file is available, it allows every character of the code to be traced back to its original position.
     */
    struct SegmentedCleanSource {
        std::string str;                    // The clean string value
        std::vector<ulong> OG_i;            // The original index of each character in the str buffer
        std::vector<ulong> OG_l;            // The original line number of each character in the str buffer
        std::vector<SourceSegment> sgm;     // The list of segments

        std::string toString();
    };
}