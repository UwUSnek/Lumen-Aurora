#pragma once
#include <string>
#include <tuple>
#include <vector>




namespace pre {
    struct LineReference {
        ulong start, len;    // The start character index of the line, relative to the buffer that contains this file and every file included by it, and the length of the line (can be 0)
        ulong OG_lineNum;    // The line number this line had in the original file
        ulong OG_fileIndex;  // The index of the file path list in which the path to the original file is saved

        LineReference(ulong _start, ulong _len, ulong _OG_lineNum, ulong _OG_fileIndex) :
            start(_start),
            len(_len),
            OG_lineNum(_OG_lineNum),
            OG_fileIndex(_OG_fileIndex) {
        }
    };




    /**
     * @brief Contains informations about the line numbers of the original file.
     *      Each element represents one OG line
     *      Its value indicates the character index in the modified file in which the OG line used to start
     */
    struct LineReferencedSource {
        std::string str;
        std::vector<LineReference> ref;

        std::string toString();
    };
}