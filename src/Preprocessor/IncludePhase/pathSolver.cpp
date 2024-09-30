#include <filesystem>
namespace fs = std::filesystem;

#include "pathSolver.hpp"
#include "Command/command.hpp"
#include "Utils/ansi.hpp"
#include "Utils/errors.hpp"








/**
 * @brief Calculates the canonical path from the raw file path used in the include directive and prints an error if it's invalid or ambiguous.
 * @param rawFilePath The path that was found in the include directive, without any modification.
 * @param curFilePatht The path of the file that is currently being preprocessed.
 * @param relevantCoords The position of the relevant section.
 * @param filePathCoords The position of the section containing the include file path.
 * @return The canonical path of the specified file.
 */
std::string pre::resolveFilePath(std::string const &rawFilePath, std::string const &curFilePath, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords) {

    // If the path is an absolute path
    if(rawFilePath[0] == '/') {

        // Validate it and return its canonical version
        utils::PathCheckResult const &result = utils::checkPath(rawFilePath);
        return validateSelectedIncludePath(rawFilePath, result, relevantCoords, filePathCoords);
    }




    // If it is a relative path
    else {
        std::vector<std::pair<std::string, utils::PathCheckResult>> validPaths;
        std::vector<std::pair<std::string, utils::PathCheckResult>> invalidPaths;

        // Check current path
        std::string const &fullPath = fs::path(curFilePath).parent_path() / rawFilePath;
        utils::PathCheckResult &&result = utils::checkPath(fullPath);
        (result.exists ? validPaths : invalidPaths).push_back(std::pair<std::string, utils::PathCheckResult>(fullPath, result));

        // Check and categorize each include path
        for(std::string const &dir : cmd::options.includePaths) {
            std::string const &fullPath = dir + "/" + rawFilePath;
            utils::PathCheckResult const &result = utils::checkPath(fullPath);

            // Skip current file path if one equivalent to it already exists. If not, push it to the correct vector
            if(result.exists) {
                std::string canonical = fs::canonical(fullPath);
                for(int i = 0;; ++i) {
                    if(i < validPaths.size() && canonical == validPaths[i].first) {
                        break;
                    }
                    if(i >= validPaths.size()) { //! Checking -1 makes it overflow and loop forever
                        validPaths.push_back(std::pair<std::string, utils::PathCheckResult>(canonical, result));
                        break;
                    }
                }
            }
            else invalidPaths.push_back(std::pair<std::string, utils::PathCheckResult>(fullPath, result));
        }


        // Print an error if no valid file path was found, listing all the paths that were tried
        if(validPaths.empty()) {
            std::string invalidPathsList = ansi::reset;
            for(ulong i = 0; i < invalidPaths.size(); ++i) {
                invalidPathsList += "\n    " + std::to_string(i + 1) + ". \"" + ansi::white + invalidPaths[i].first + ansi::reset + "\"";
            }
            printError(
                ErrorCode::ERROR_PRE_PATH_NOT_FOUND,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Could not open file \"" + rawFilePath + "\": no such file or directory.\n" +
                "Paths tried: " + invalidPathsList
            );
        }

        // Print an error if more than one valid file path was found
        if(validPaths.size() > 1) {
            std::string validPathsList = ansi::reset;
            for(ulong i = 0; i < validPaths.size(); ++i) {
                validPathsList += "\n    " + std::to_string(i + 1) + ". \"" + ansi::white + validPaths[i].first + ansi::reset + "\"";
            }
            printError(
                ErrorCode::ERROR_PRE_PATH_AMBIGUOUS,
                utils::ErrType::PREPROCESSOR,
                relevantCoords,
                filePathCoords,
                "Ambiguous file path \"" + rawFilePath + "\".\n" +
                "Files that match this path: " + validPathsList
            );
        }


        // If only one valid file path was found
        else {
            return validateSelectedIncludePath(validPaths[0].first, validPaths[0].second, relevantCoords, filePathCoords);
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
std::string pre::validateSelectedIncludePath(std::string const &filePath, utils::PathCheckResult const &checkResult, ElmCoords const &relevantCoords, ElmCoords const &filePathCoords) {

    // Print an error if the file doesn't exist
    if(!checkResult.exists) {
        printError(
            ErrorCode::ERROR_PRE_PATH_NOT_FOUND,
            utils::ErrType::PREPROCESSOR,
            relevantCoords,
            filePathCoords,
            "Could not open file \"" + filePath + "\": no such file or directory.\n" +
            "File path was interpreted as: \"" + ansi::white + filePath + ansi::reset + "\".\n" +
            "Make sure that the path is correct and the file exists."
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
            "File path was interpreted as: \"" + ansi::white + actualFilePath + ansi::reset + "\"."
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
            "File path was interpreted as: \"" + ansi::white + actualFilePath + ansi::reset + "\"."
        );
    }

    // Reutrn canonical path
    return actualFilePath;
}