#include <iostream>
#include <cstring>
#include "command.hpp"
#include "Utils/utils.hpp"






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
        ulong optionPosition = 0;  // The character index of the current option relative to the full command string
        ElmCoordsCL lastOutputTypeCoords(0, 0);
        ElmCoordsCL lastTargetPlatformCoords(0, 0);
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
                                ErrorCode::ERROR_CMD_INCOMPATIBLE_OPTIONS,
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


                        // Valid option
                        if(i < argc - 1) {
                            options.outputFile = argv[i + 1];
                            ++i;
                            optionPosition += strlen(argv[i]);
                        }

                        // Missing output path
                        else {
                            utils::printErrorCL(
                                ErrorCode::ERROR_CMD_NO_OUTPUT_FILE,
                                ElmCoordsCL(optionPosition, optionPosition + o.length()),
                                ElmCoordsCL(optionPosition, optionPosition + o.length()),
                                "Missing output path after " + o + " option.\n" +
                                "An output path must be specified.",
                                DBG_fullCommand
                            );
                        }

                        break;
                    }


                    // Parse target platform
                    for(char c : "lw") if(o[1] == c) {

                        // Incompatible target platform
                        ElmCoordsCL currentTargetPlatformCoords = ElmCoordsCL(optionPosition, optionPosition + 2);
                        if(lastTargetPlatformCoords.end) {
                            utils::printErrorCL(
                                ErrorCode::ERROR_CMD_INCOMPATIBLE_OPTIONS,
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
                    }
                }
                else if(o == "--help") {
                    options.isHelp = true;
                }
                else if(o == "--version") {
                    options.isVersion = true;
                }
                else {
                    utils::printErrorCL(
                        ErrorCode::ERROR_CMD_UNKNOWN_OPTION,
                        cmd::ElmCoordsCL(0, 0),
                        cmd::ElmCoordsCL(0, 0),
                        "Unknown command line option \"" + ansi::white + o + ansi::reset + "\".",
                        DBG_fullCommand
                    );
                }
            }


            // Source file
            else {
                options.sourceFile = o;
            }


            optionPosition += o.length() + 1;
        }




        // Print error if no source file is specified
        if(options.sourceFile.empty()) {
            utils::printErrorCL(
                ErrorCode::ERROR_CMD_NO_SOURCE_FILE,
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



    //FIXME parse --no-color
    //FIXME parse --no-display
    //FIXME parse --no-error
    //FIXME parse --no-status
    //FIXME parse --silent

    //FIXME parse -i
    //FIXME parse -I

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


        std::cout <<
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
    }








    void printVersion() {
        //FIXME print version. get version number dynamically
        std::cout << ansi::bold_magenta << "ALC" << ansi::reset << " (Aurora/Lumen Compiler)\n";
        std::cout << ansi::bold_magenta << "Version:" << ansi::reset << " L-0.1.0";
        // std::cout ansi::bold_magenta << << "Version:" << ansi::reset << " W-0.1.0"; //TODO windows build
        // std::cout ansi::bold_magenta << << "Version:" << ansi::reset << " X-0.1.0"; //TODO mac build (prob not gonna happen)
    }
}