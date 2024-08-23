#pragma once
#include <string>
#include <tuple>
#include <vector>




namespace pre {
    struct SourceSegment {
        // ulong start, len;    // The start index and length (0 if deleted) of the segment, relative to the clean buffer that contains this file and every file included by it
        ulong OG_start, len;    // The start index and length of the segment, relative to the original file
        ulong OG_fileIndex;  // The index of the file path list in which the path to the original file is saved
        bool isRemoved;      // Whether the segment was removed during the cleanup phase

        SourceSegment(ulong _OG_start, ulong _len, ulong _OG_fileIndex, bool _isRemoved) :
            OG_start(_OG_start),
            len(_len),
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
        std::string str;
        std::vector<SourceSegment> sgm;

        std::string toString();
    };
}