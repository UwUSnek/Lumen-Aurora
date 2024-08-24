#include <fstream>
#include <iomanip>
#include "SegmentedCleanSource.hpp"
#include "Utils/utils.hpp"
#include "Preprocessor/preprocessor.hpp"



namespace pre {
    //TODO improve this function or just remove it, its prob incredibly inefficient
    std::string SegmentedCleanSource::toString() {
        std::stringstream r;
        // ulong cleanIndex = 0;


        for(ulong i = 0; i < str.length(); ++i) {
            // Print last deleted segment if present //FIXME check if this works with include adjacent deleted sections
            if(i > 0 && og[i].f == og[i - 1].f && og[i].i - og[i - 1].i > 1) {
                //TODO
            }

            // Print current character
            else {
                char c = str[i];
                r << utils::formatChar(c, true);
                if(c == '\n') r << "\n";
            }
        }


        // // For each segment
        // for(ulong i = 0; i < sgm.size(); ++i) {

        //     // If the segment was removed, get its value from the original file and print it
        //     if(sgm[i].isRemoved) {

        //         // Open original file and jump to the segment's starting index
        //         std::ifstream OG_f(sourceFilePaths[sgm[i].OG_fileIndex], std::ios::binary);
        //         OG_f.seekg(sgm[i].OG_start, std::ios::beg);

        //         // Print every character one by one
        //         r << ansi::black;
        //         for(ulong j = 0; j < sgm[i].len; ++j) {
        //             char OG_c; OG_f.get(OG_c);
        //             r << utils::formatChar(OG_c, true) << ansi::black;
        //             if(OG_c == '\n') r << "\n";
        //         }
        //         r << ansi::reset;

        //         // Close the original file
        //         OG_f.close();
        //     }


        //     // If not, read it from the clean string buffer
        //     else {
        //         ulong end = cleanIndex + sgm[i].len;  //! This is called "end" but it's actually the last index + 1
        //         for(; cleanIndex < end; ++cleanIndex) {
        //             char c = str[cleanIndex];
        //             r << utils::formatChar(c, true);
        //             if(c == '\n') r << "\n";
        //         }
        //     }
        // }

        return r.str();
    }
}