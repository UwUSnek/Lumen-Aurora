#pragma once
#include <string>
#include "Utils/utils.hpp"
#include "Preprocessor/ElmCoords.hpp"




namespace pre {
    std::string             resolveFilePath(const std::string &rawFilePath,         const std::string &curfilePath, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords);
    std::string validateSelectedIncludePath(const std::string &filePath, utils::PathCheckResult const &checkResult, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords);
}