#pragma once
#include <string>
#include <tuple>
#include <vector>




namespace pre {
    struct SourceSegment {
        ulong start, len;    // The start character index of the line, relative to the buffer that contains this file and every file included by it, and the length of the line (can be 0)
        ulong OG_fileIndex;  // The index of the file path list in which the path to the original file is saved
        bool isRemoved;      // Whether the segment has been removed by the cleanup phase

        SourceSegment(ulong _start, ulong _len, ulong _OG_fileIndex, bool _isRemoved) :
            start(_start),
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