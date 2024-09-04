#pragma once
#include <string>
#include <vector>

#include "Utils/StringPipe.hpp"




namespace pre {
    enum class CleanSourceType {
        MISC,
        STRING,
        CHAR
    };




    /**
     * @brief A structure that contains informations about a single character
     */
    struct CleanSourceMeta {
        CleanSourceType type;  // The type of the segment the character is part of
        ulong i;               // The original index of the character in the str buffer
        ulong l;               // The original line number of the character in the str buffer
        ulong f;               // The index of the original file of the character in the str buffer

        CleanSourceMeta(CleanSourceType _type, ulong _i, ulong _l, ulong _f) :
            type(_type),
            i(_i),
            l(_l),
            f(_f) {
        }

        CleanSourceMeta(CleanSourceType _type, CleanSourceMeta &meta) :
            type(_type),
            i(meta.i),
            l(meta.l),
            f(meta.f) {
        }
    };





    /**
     * @brief This structure contain source code that has gone through the cleanup phase.
     *      It retains informations about any removed portion of code.
     *      If the original file is available, it allows every character of the code to be traced back to its original position.
     */
    struct SegmentedCleanSource {
        StringPipe str;                     // The clean string value
        std::vector<CleanSourceMeta> meta;  // Informations about each character

        std::string toString();
        std::string toStringVerbose();
    };
}