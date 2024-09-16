#include <cstring>
#include <filesystem>
namespace fs = std::filesystem;

#include "command.hpp"
#include "ALC.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"








cmd::Options cmd::options;




/**
 * @brief Parses the command line arguments and initializes cmd::options
 * @param argc Main function's argc
 * @param argv Main function's argv
 * @param DBG_fullCommand The full command as a string. This is exclusively used for error messages and never parsed.
 */
void cmd::parseOptions(int argc, char* argv[], std::string DBG_fullCommand){

    // Loop the options and parse them
    ulong optionPosition = strlen(argv[0]) + 1;  // The character index of the current option relative to the full command string
    ElmCoordsCL lastOutputTypeCoords(0, 0);
    ElmCoordsCL lastTargetPlatformCoords(0, 0);
    ElmCoordsCL lastSourceCoords(0, 0);
    for(int i = 1; i < argc; ++i) {
        std::string o(argv[i]);


        // Options
        if(o[0] == '-') {
            if(o.length() == 2) {


                // Parse output type and path
                for(char c : "pmx") if(o[1] == c) {

                    // Incompatible output type
                    ElmCoordsCL currentOutputTypeCoords = ElmCoordsCL(optionPosition, optionPosition + 2);
                    if(lastOutputTypeCoords.end) {
                        utils::printErrorCL(
                            ERROR_CMD_INCOMPATIBLE_OPTIONS,
                            lastOutputTypeCoords,
                            currentOutputTypeCoords,
                            "Incompatible options: " +
                            DBG_fullCommand.substr(currentOutputTypeCoords.start, currentOutputTypeCoords.end - currentOutputTypeCoords.start) + " option used with " +
                            DBG_fullCommand.substr(   lastOutputTypeCoords.start,    lastOutputTypeCoords.end    - lastOutputTypeCoords.start),
                            DBG_fullCommand
                        );
                    }
                    options.outputType = c;
                    lastOutputTypeCoords = currentOutputTypeCoords;


                    // Missing output path
                    ++i;
                    if(i >= argc) {
                        utils::printErrorCL(
                            ERROR_CMD_NO_OUTPUT_FILE,
                            ElmCoordsCL(optionPosition, optionPosition + o.length()),
                            ElmCoordsCL(optionPosition, optionPosition + o.length()),
                            "Missing output path after " + o + " option.\n" +
                            "An output path must be specified.",
                            DBG_fullCommand
                        );
                    }

                    // Valid option
                    options.outputFile = argv[i];
                    optionPosition += strlen(argv[i]) + 1;

                    break;
                }


                // Parse target platform
                for(char c : "lw") if(o[1] == c) {

                    // Incompatible target platform
                    ElmCoordsCL currentTargetPlatformCoords = ElmCoordsCL(optionPosition, optionPosition + 2);
                    if(lastTargetPlatformCoords.end) {
                        utils::printErrorCL(
                            ERROR_CMD_INCOMPATIBLE_OPTIONS,
                            lastTargetPlatformCoords,
                            currentTargetPlatformCoords,
                            "Incompatible options: " +
                            DBG_fullCommand.substr(currentTargetPlatformCoords.start, currentTargetPlatformCoords.end - currentTargetPlatformCoords.start) + " option used with " +
                            DBG_fullCommand.substr(   lastTargetPlatformCoords.start,    lastTargetPlatformCoords.end    - lastTargetPlatformCoords.start),
                            DBG_fullCommand
                        );
                    }
                    options.outputType = c;
                    lastTargetPlatformCoords = currentTargetPlatformCoords;

                    break;
                }


                // Parse include and import paths
                for(char c : "iI") if(o[1] == c) {
                    std::string w = c == 'I' ? "nclude" : "mport";
                    cmd::ElmCoordsCL optionCoords(optionPosition, optionPosition + o.length());
                    ulong pathPosition = optionPosition + o.length() + 1;

                    ++i;
                    if(i >= argc) {
                        utils::printErrorCL(
                            c == 'I' ? ERROR_CMD_INCLUDE_PATH_MISSING : ERROR_CMD_IMPORT_PATH_MISSING,
                            optionCoords,
                            optionCoords,
                            "Missing i" + w + "path after " + o + " option.\n"
                            "A path to a directory must be specified.",
                            DBG_fullCommand
                        );
                    }

                    // If path doesn't exist, print an error
                    std::string path(argv[i]);
                    utils::PathCheckResult result = utils::checkPath(argv[i]);
                    if(!result.exists) {
                        utils::printErrorCL(
                            c == 'I' ? ERROR_CMD_INCLUDE_PATH_NOT_FOUND : ERROR_CMD_IMPORT_PATH_NOT_FOUND,
                            optionCoords,
                            cmd::ElmCoordsCL(pathPosition, pathPosition + path.length()),
                            "Could not find directory \"" + path + "\".\n" +
                            "Current working directory is: \"" + ansi::white + fs::current_path().string() + ansi::reset + "\".",
                            DBG_fullCommand
                        );
                    }

                    // If it is a directory, print an error
                    std::string canonicalPath = fs::canonical(path);
                    if(!result.isDir) {
                        utils::printErrorCL(
                            c == 'I' ? ERROR_CMD_INCLUDE_PATH_IS_FILE : ERROR_CMD_IMPORT_PATH_IS_FILE,
                            optionCoords,
                            cmd::ElmCoordsCL(pathPosition, pathPosition + path.length()),
                            "The specified i" + w + " path \"" + path + "\" is not a directory.\n" +
                            "I" + w + " path was interpreted as \"" + ansi::white + canonicalPath + ansi::reset + "\".",
                            DBG_fullCommand
                        );
                    }

                    // If it has no read permission, print an error
                    if(!result.canRead) {
                        utils::printErrorCL(
                            c == 'I' ? ERROR_CMD_INCLUDE_PATH_NO_PERMISSION : ERROR_CMD_IMPORT_PATH_NO_PERMISSION,
                            optionCoords,
                            cmd::ElmCoordsCL(pathPosition, pathPosition + path.length()),
                            "The specified i" + w + " directory \"" + path + "\" cannot be used: no read permission.\n" +
                            "I" + w + " path was interpreted as \"" + ansi::white + canonicalPath + ansi::reset + "\".",
                            DBG_fullCommand
                        );
                    }


                    // Push canonical path to the list and adjust the option position
                    (c == 'I' ? options.includePaths : options.importPaths).push_back(canonicalPath);
                    optionPosition += path.length() + 1;

                    break;
                }
            }




            else if(o == "--help") {
                options.isHelp = true;
            }
            else if(o == "--version") {
                options.isVersion = true;
            }




            else if(o == "--no-color") {
                options.printColor = false;
            }
            else if(o == "--no-display") {
                options.printDisplay = false;
            }
            else if(o == "--no-errors") {
                options.printErrors = false;
            }
            else if(o == "--no-status") {
                options.printStatus = false;
            }
            else if(o == "--silent") {
                options.silent = true;
                options.printStatus = false;
            }




            else {
                //TODO look for similar existing options and suggest them
                //TODO add a list of possible intended options that aren't necessarily close by value
                utils::printErrorCL(
                    ERROR_CMD_UNKNOWN_OPTION,
                    cmd::ElmCoordsCL(optionPosition, optionPosition + o.length()),
                    cmd::ElmCoordsCL(optionPosition, optionPosition + o.length()),
                    "Unknown command line option \"" + ansi::white + o + ansi::reset + "\".",
                    DBG_fullCommand
                );
            }
        }


        // Source file
        else {

            // Print error if multiple sources are specified
            cmd::ElmCoordsCL currentSourceCoords(optionPosition, optionPosition + o.length());
            if(lastSourceCoords.end > 0) {
                utils::printErrorCL(
                    ERROR_CMD_MULTIPLE_SOURCES,
                    lastSourceCoords,
                    currentSourceCoords,
                    "Multiple source files specified.\n"
                    "Only one at a time is allowed.",
                    DBG_fullCommand
                );
                lastSourceCoords = currentSourceCoords;
            }

            // Print error if the source doesnt exist
            utils::PathCheckResult result = utils::checkPath(o);
            if(!result.exists) {
                utils::printErrorCL(
                    ERROR_CMD_SOURCE_INVALID,
                    currentSourceCoords,
                    currentSourceCoords,
                    "Could not find source file \"" + o + "\".\n" +
                    "Current working directory is: \"" + ansi::white + fs::current_path().string() + ansi::reset + "\".",
                    DBG_fullCommand
                );
            }

            // If it is a directory, print an error
            std::string canonicalPath = fs::canonical(o);
            if(result.isDir) {
                utils::printErrorCL(
                    ERROR_CMD_INCLUDE_PATH_IS_FILE,
                    currentSourceCoords,
                    currentSourceCoords,
                    "The specified source file path \"" + o + "\" is a directory.\n" +
                    "Source file path was interpreted as \"" + ansi::white + canonicalPath + ansi::reset + "\".",
                    DBG_fullCommand
                );
            }

            // Print error if the source doesnt have read permission
            if(!result.canRead) {
                utils::printErrorCL(
                    ERROR_CMD_SOURCE_INVALID,
                    currentSourceCoords,
                    currentSourceCoords,
                    "Could not open source file \"" + o + "\": no read permission.\n" +
                    "Source file path was interpreted as \"" + ansi::white + canonicalPath + ansi::reset + "\".",
                    DBG_fullCommand
                );
            }

            options.sourceFile = canonicalPath;
        }


        // Adjust option position index (additional paths or values are skipped in their respective sections)
        optionPosition += o.length() + 1;
    }




    // Print error if no source file is specified
    if(options.sourceFile.empty()) {
        utils::printErrorCL(
            ERROR_CMD_SOURCE_MISSING,
            ElmCoordsCL(optionPosition - 1, optionPosition - 1),
            ElmCoordsCL(optionPosition - 1, optionPosition - 1),
            "Source code path is missing.\n"
            "A path to the input source code must be specified.",
            DBG_fullCommand
        );
    }




    // Use default type and path if not specified
    //! Outpuut path is tied to the output type option
    if(options.outputType == '\0') {
        options.outputFile = options.sourceFile + ".out";
        options.outputType = 'x';
    }




    // Use default target platform if not specified
    if(options.targetPlatform == '\0') {
        options.targetPlatform = 'l';
    }
}


//FIXME parse --o-none
//FIXME parse --o-unused
//FIXME parse --o-rewrite
//FIXME parse --o-thread
//FIXME parse --o-memory