#include "ALC.hpp"
#include <cstring>
#include <filesystem>
namespace fs = std::filesystem;

#include "command.hpp"
#include "ALC.hpp"
#include "Utils/utils.hpp"
#include "Utils/errors.hpp"





namespace cmd {
    cmd::Options options;




    /**
     * @brief Parses the command line arguments and initializes cmd::options
     * @param argc Main function's argc
     * @param argv Main function's argv
     * @param DBG_fullCommand The full command as a string. This is exclusively used for error messages and never parsed.
     */
    void parseOptions(int argc, char* argv[], std::string DBG_fullCommand){

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
                //TODO CHECK THIS TOO IN THE INCLUDE STUFF but the other way around
                //TODO                         if(!fs::is_directory(fs::is_symlink(canonicalPath) ? fs::read_symlink(canonicalPath).string() : canonicalPath)) {




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








    //TODO update compiler executable name
    void printHelp(){
        std::string none______        = "\033[90m │ ";
        std::string default1_         = ansi::reset + ansi::fill_magenta + " "         + ansi::reset + " ";
        std::string default__________ = ansi::reset + ansi::fill_magenta + " DEFAULT " + ansi::reset + " ";


        consoleLock.lock();
        cout <<
            ansi::bold_magenta << "Command syntax: " << ansi::reset << "alc <source> <options...>\n" <<
            ansi::bold_magenta << "          source │ " << ansi::reset << "The path to the file containing the source code                                        " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "                 │ " << ansi::reset << "                                                                                       " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "          --help │ " << ansi::reset << "Print this message and ignore any other option but --no-color                          " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "       --version │ " << ansi::reset << "Print the version and ignore any other option but --no-color or --help                 " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "                 │ " << ansi::reset << "                                                                                       " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "      --no-color │ " << ansi::reset << "Disable colors and styling in the compiler's output                                    " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "    --no-display │ " << ansi::reset << "Disable source code display in error messages                                          " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "     --no-errors │ " << ansi::reset << "Don't print error messages (doesn't affect exit code)                                  " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "     --no-status │ " << ansi::reset << "Disable real-time status and progress bar in the compiler's output                     " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "        --silent │ " << ansi::reset << "Only print error messages without showing any additional information                   " << none______ << ansi::bright_black << "activates --no-status\n" << ansi::reset <<
            ansi::bold_magenta << "                 │ " << ansi::reset << "                                                                                       " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "       -p <path> │ " << ansi::reset << "Preprocess the file <source> (without compiling it), write the output in <path>         " << default1_ << ansi::bright_black << "incompatible with -m and -x\n" << ansi::reset <<
            ansi::bold_magenta << "       -m <path> │ " << ansi::reset << "Compile the file <source> into a module <path> (without creating an executable)         " << default1_ << ansi::bright_black << "incompatible with -p and -x\n" << ansi::reset <<
            ansi::bold_magenta << "       -x <path> │ " << ansi::reset << "Compile the file <source> into an executable file <path>                        " << default__________ << ansi::bright_black << "incompatible with -p and -m\n" << ansi::reset <<
            ansi::bold_magenta << "                 │ " << ansi::reset << "                                                                                       " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "       -i <path> │ " << ansi::reset << "Use import path <path> (does not affect include paths)                                 " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "       -I <path> │ " << ansi::reset << "Use include path <path> (does not affect import paths)                                 " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "                 │ " << ansi::reset << "                                                                                       " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "              -l │ " << ansi::reset << "Compile for Linux systems                                                       " << default__________ << ansi::bright_black << "requires -x, incompatible with -l\n" << ansi::reset <<
            ansi::bold_magenta << "              -w │ " << ansi::reset << "Compile for Windows systems                                                             " << default1_ << ansi::bright_black << "requires -x, incompatible with -w\n" << ansi::reset <<
            ansi::bold_magenta << "                 │ " << ansi::reset << "                                                                                       " << none______ << ansi::bright_black << "\n" <<
            ansi::bold_magenta << "        --o-none │ " << ansi::reset << "Turn off all optimizations                                                      " << default__________ << ansi::bright_black << "incompatible with other --o options\n" << ansi::reset <<
            ansi::bold_magenta << "      --o-unused │ " << ansi::reset << "Allow removing unused code                                                              " << default1_ << ansi::bright_black << "incompatible with --o-none\n" << ansi::reset <<
            ansi::bold_magenta << "     --o-rewrite │ " << ansi::reset << "Allow rewriting code to optimize it                                                     " << default1_ << ansi::bright_black << "incompatible with --o-none\n" << ansi::reset <<
            ansi::bold_magenta << "      --o-thread │ " << ansi::reset << "Use thread pool                                                                         " << default1_ << ansi::bright_black << "incompatible with --o-none\n" << ansi::reset <<
            ansi::bold_magenta << "      --o-memory │ " << ansi::reset << "Use memory pool                                                                         " << default1_ << ansi::bright_black << "incompatible with --o-none\n" << ansi::reset <<
        "";
        consoleLock.unlock();
    }








    void printVersion() {
        consoleLock.lock();
        //FIXME print version. get version number dynamically
        cout << ansi::bold_magenta << "ALC" << ansi::reset << " (Aurora/Lumen Compiler)\n";
        cout << ansi::bold_magenta << "Version:" << ansi::reset << " L-0.1.0";
        // cout ansi::bold_magenta << << "Version:" << ansi::reset << " W-0.1.0"; //TODO windows build
        // cout ansi::bold_magenta << << "Version:" << ansi::reset << " X-0.1.0"; //TODO mac build (prob not gonna happen)
        consoleLock.unlock();
    }
}