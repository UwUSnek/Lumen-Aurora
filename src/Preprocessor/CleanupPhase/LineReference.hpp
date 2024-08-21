#pragma once
#include <string>
#include <tuple>
#include <vector>




namespace pre {
    /**
     * @brief Contains informations about the line numbers of the original file.
     *      Each element represents one OG line
     *      Its value indicates the character index in the modified file in which the OG line used to start
     */
    typedef std::vector<unsigned long> LineReference;


    std::string lineReferencedDataToString(std::pair<std::string, LineReference> b);
}