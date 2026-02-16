#include <filesystem>
#include <utility>
#include "pathSolver.hpp"
#include "Command/command.hpp"
#include "Utils/ansi.hpp"
#include "Main/errors.hpp"
#include "Utils/utils.hpp"

namespace fs = std::filesystem;



//TODO comment
//a canonical path
//it contains a string that represents the canonical path and a list of strings representing the valid formed paths that lead to it and their check result.
struct CanonicalPathData {
    std::string canonical;
    std::vector<std::pair<std::string, utils::PathCheckResult>> contributors = std::vector<std::pair<std::string, utils::PathCheckResult>>();

    explicit CanonicalPathData(const std::string &_canonical) :
        canonical(_canonical) {
    }

    void addContributor(const std::string &path, const utils::PathCheckResult checkResult) {
        contributors.emplace_back(path, checkResult);
    }
};

//TODO comment
// A list of CanonicalPathData
// this struct handles canonical path calculation and deduplication.
// use addPath()
struct CanonicalPathList {
    std::vector<CanonicalPathData> paths = std::vector<CanonicalPathData>();

    CanonicalPathList() = default;
    void addPath(const std::string &rawPath, const utils::PathCheckResult checkResult) {
        auto canonical = fs::path(rawPath).lexically_normal().string();

        // If the path is already present, register it as a contributor
        for(auto &pathData : paths) {
            if(pathData.canonical == canonical) {
                pathData.addContributor(rawPath, checkResult);
                return;
            }
        }

        // If the path is not present, add a new entry and register the first contributor
        paths.emplace_back(canonical);
        paths.back().addContributor(rawPath, checkResult);
    }

    ulong size() const {
        return paths.size();
    }

    bool empty() const {
        return paths.empty();
    }

    CanonicalPathData& operator[](size_t index) {
        return paths[index];
    }

    const CanonicalPathData& operator[](size_t index) const {
        return paths[index];
    }
};







/**
 * @brief Calculates the canonical path from the raw file path used in the include directive and prints an error if it's invalid or ambiguous.
 * @param rawFilePath The path that was found in the include directive, without any modification.
 * @param curFilePatht The path of the file that is currently being preprocessed.
 * @param relevantCoords The position of the relevant section.
 * @param filePathCoords The position of the section containing the include file path.
 * @return The canonical path of the specified file.
 */
std::string pre::resolveFilePath(const std::string &rawFilePath, const std::string &curFilePath, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords) { //NOSONAR

    // If the path is an absolute path
    if(rawFilePath[0] == '/') {

        // Validate it and return its canonical version
        utils::PathCheckResult const &result = utils::checkPath(rawFilePath);
        return validateSelectedIncludePath(rawFilePath, result, relevantCoords, filePathCoords);
    }




    // If it is a relative path
    else {
        CanonicalPathList validPaths;
        CanonicalPathList invalidPaths;


        // Check the source's parent directory
        {
            const std::string &fullPath = fs::path(curFilePath).parent_path() / rawFilePath;
            utils::PathCheckResult &&result = utils::checkPath(fullPath);
            (result.exists ? validPaths : invalidPaths).addPath(fullPath, result);
        }


        // Check and categorize each include path
        for(const std::string &dir : cmd::options.includePaths) {
            const std::string &fullPath = fs::path(dir) / rawFilePath;
            utils::PathCheckResult const &result = utils::checkPath(fullPath);
            (result.exists ? validPaths : invalidPaths).addPath(fullPath, result);
        }


        // Print an error if no valid file path was found, listing all the paths that were tried
        if(validPaths.empty()) {
            std::string invalidPathsList = ansi::reset;
            for(ulong i = 0; i < invalidPaths.size(); ++i) {
                auto pathIndexStr = std::format("    {}. ", std::to_string(i + 1));
                invalidPathsList += std::format(
                    "\n{}\"{}{}{}\"",
                    pathIndexStr,
                    ansi::white, invalidPaths[i].canonical, ansi::reset
                );
                for(const auto &[contributor, result] : invalidPaths[i].contributors) {
                    invalidPathsList += std::format(
                        "\n    {:{}}{}\"{}\"{}",
                        "", pathIndexStr.size(),
                        ansi::bright_black, contributor, ansi::reset
                    );
                }
            }
            printError(
                ErrorCode::ERROR_PRE_PATH_NOT_FOUND,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Could not open file \"" + rawFilePath + "\": no such file or directory.\n" +
                "Paths tried: " + invalidPathsList,
                true //TODO recovery system. skip to the first token that makes sense
            );
        }


        // Print an error if more than one valid file path was found
        if(validPaths.size() > 1) {
            std::string validPathsList = ansi::reset;
            for(ulong i = 0; i < validPaths.size(); ++i) {
                auto pathIndexStr = std::format("    {}. ", std::to_string(i + 1));
                validPathsList += std::format(
                    "\n{}\"{}{}{}\"",
                    pathIndexStr,
                    ansi::white, validPaths[i].canonical, ansi::reset
                );
                for(const auto &[contributor, result] : validPaths[i].contributors) {
                    validPathsList += std::format(
                        "\n    {:{}}{}\"{}\"{}",
                        "", pathIndexStr.size(),
                        ansi::bright_black, contributor, ansi::reset
                    );
                }
            }
            printError(
                ErrorCode::ERROR_PRE_PATH_AMBIGUOUS,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Ambiguous file path \"" + rawFilePath + "\".\n" +
                "Files that match this path: " + validPathsList,
                true //TODO recovery system. skip to the first token that makes sense
            );
        }


        // If only one valid file path was found
        else {
            return validateSelectedIncludePath(
                validPaths[0].canonical,
                validPaths[0].contributors[0].second,
                relevantCoords,
                filePathCoords
            );
        }
    }


    //! Bogus return statement so GCC doesn't cry about it
    return "";
}








/**
 * @brief Prints an error if the path cannot be opened, is a directory or the compiler doesn't have read permissions on it.
 * @param filePath The path of the file.
 * @param checkResult The result of the check performed on the file.
 * @param relevantCoords The position of the relevant section.
 * @param filePathCoords The position of the section containing the include file path.
 * @return The canonical path of the file at <filePath>.
 */
std::string pre::validateSelectedIncludePath(const std::string &filePath, utils::PathCheckResult const &checkResult, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords) {

    // Print an error if the file doesn't exist
    if(!checkResult.exists) {
        printError(
            ErrorCode::ERROR_PRE_PATH_NOT_FOUND,
            utils::ErrType::PREPROCESSOR,
            relevantCoords,
            filePathCoords,
            "Could not open file \"" + filePath + "\": no such file or directory.\n" +
            "File path was interpreted as: \"" + ansi::white + filePath + ansi::reset + "\".\n" +
            "Make sure that the path is correct and the file exists.",
            true //TODO recovery system. skip to the first token that makes sense
        );
    }

    // Print an error if the file doesn't have read permission
    std::string actualFilePath = fs::canonical(filePath);
    if(!checkResult.canRead) {
        printError(
            ErrorCode::ERROR_PRE_PATH_NO_PERMISSION,
            utils::ErrType::PREPROCESSOR,
            relevantCoords,
            filePathCoords,
            "Could not open file \"" + filePath + "\": no read permission.\n" +
            "File path was interpreted as: \"" + ansi::white + actualFilePath + ansi::reset + "\".",
            true //TODO recovery system. skip to the first token that makes sense
        );
    }

    // Print an error if the file is a directory
    if(checkResult.isDir) {
        printError(
            ErrorCode::ERROR_PRE_PATH_IS_DIRECTORY,
            utils::ErrType::PREPROCESSOR,
            relevantCoords,
            filePathCoords,
            "Could not include the specified path: \"" + filePath + "\" is a directory.\n" +
            "File path was interpreted as: \"" + ansi::white + actualFilePath + ansi::reset + "\".",
            true //TODO recovery system. skip to the first token that makes sense
        );
    }

    // Reutrn canonical path
    return actualFilePath;
}