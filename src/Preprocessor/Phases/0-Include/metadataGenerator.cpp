#include "Preprocessor/SegmentedCleanSource.hpp"
#include "metadataGenerator.hpp"








//FIXME rename segmentedcleansource to something better.
//FIXME it's just source code with metadata
void pre::generateMetadata(std::string rawCode, ptr<SegmentedCleanSource> r, ulong DBG_filePathIndex) {
    ulong i = 0; // The current character index. Starts from 0
    ulong c = 0; // The current column number.   Starts from 0
    ulong l = 0; // The current line number.     Starts from 0


    // For each character in the string
    while(i < rawCode.length()) {

        // Save the metadata in the return pipe
        r->str += rawCode[i];
        r->meta += CleanSourceMeta(i, l, c, DBG_filePathIndex);

        // Calculate metadata for the next character
        if(rawCode[i] == '\n') {
            ++l;
            c = 0;
        }
        ++i;
        ++c;
    }


    // Close the pipes. All of the code has had its metadata generated
    r->str. closePipe();
    r->meta.closePipe();
}