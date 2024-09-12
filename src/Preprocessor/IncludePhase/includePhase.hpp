#pragma once
#include <string>
#include "Preprocessor/CleanupPhase/SegmentedCleanSource.hpp"



namespace pre {
    void startIncludePhase(SegmentedCleanSource *b, SegmentedCleanSource *r);
    void parseIncludeStatementName(ulong index, pre::SegmentedCleanSource *b, std::string &match);
    void parseIncludeStatementPath(ulong index, pre::SegmentedCleanSource *b, std::string &filePathMatch);

    std::string             resolveFilePath(std::string const &rawFilePath,         std::string const &curfilePath, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords);
    std::string validateSelectedIncludePath(std::string const &filePath, utils::PathCheckResult const &checkResult, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords);
}