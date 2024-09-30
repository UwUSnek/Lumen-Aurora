#pragma once
#include <string>
#include "Utils/utils.hpp"
#include "Preprocessor/ElmCoords.hpp"




namespace pre {
    std::string             resolveFilePath(std::string const &rawFilePath,         std::string const &curfilePath, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords);
    std::string validateSelectedIncludePath(std::string const &filePath, utils::PathCheckResult const &checkResult, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords);
}