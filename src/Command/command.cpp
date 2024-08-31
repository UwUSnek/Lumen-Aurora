#include <iostream>
#include <cstring>
#include "command.hpp"
#include "Utils/utils.hpp"









namespace cmd {
    Options parseOptions(int argc, char* argv[]){
        Options r;




        // Recreate full command
        std::string fullCommand;
        for(int i = 1; i < argc; ++i) {
            if(i) fullCommand += " ";
            fullCommand += argv[i];
        }




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
                                fullCommand.substr(currentOutputTypeCoords.start, currentOutputTypeCoords.end - currentOutputTypeCoords.start) + " option used with " +
                                fullCommand.substr(   lastOutputTypeCoords.start,    lastOutputTypeCoords.end    - lastOutputTypeCoords.start),
                                fullCommand
                            );
                        }
                        r.outputType = c;
                        lastOutputTypeCoords = currentOutputTypeCoords;


                        // Valid option
                        if(i < argc - 1) {
                            r.outputFile = argv[i + 1];
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
                                fullCommand
                            );
                        }

                        break;
                    }


                    // Parse target platform
                    for(char c : "lw") if(o[i] == c) {

                        // Incompatible target platform
                        ElmCoordsCL currentTargetPlatformCoords = ElmCoordsCL(optionPosition, optionPosition + 2);
                        if(lastTargetPlatformCoords.end) {
                            utils::printErrorCL(
                                ErrorCode::ERROR_CMD_INCOMPATIBLE_OPTIONS,
                                lastTargetPlatformCoords,
                                currentTargetPlatformCoords,
                                "Incompatible options: " +
                                fullCommand.substr(currentTargetPlatformCoords.start, currentTargetPlatformCoords.end - currentTargetPlatformCoords.start) + " option used with " +
                                fullCommand.substr(   lastTargetPlatformCoords.start,    lastTargetPlatformCoords.end    - lastTargetPlatformCoords.start),
                                fullCommand
                            );
                        }
                        r.outputType = c;
                        lastTargetPlatformCoords = currentTargetPlatformCoords;
                    }
                }
                else if(o == "--help") {
                    r.isHelp = true;
                }
                else if(o == "--version") {
                    r.isVersion = true;
                }
                else {
                    utils::printErrorCL(
                        ErrorCode::ERROR_CMD_UNKNOWN_OPTION,
                        cmd::ElmCoordsCL(0, 0),
                        cmd::ElmCoordsCL(0, 0),
                        "Unknown command line option \"" + ansi::white + o + ansi::reset + "\".",
                        fullCommand
                    );
                }
            }


            // Source file
            else {
                r.sourceFile = o;
            }


            optionPosition += o.length() + 1;
        }




        // Print error if no source file is specified
        if(r.sourceFile.empty()) {
            utils::printErrorCL(
                ErrorCode::ERROR_CMD_NO_SOURCE_FILE,
                ElmCoordsCL(optionPosition - 1, optionPosition - 1),
                ElmCoordsCL(optionPosition - 1, optionPosition - 1),
                "Source code path is missing.\n"
                "A path to the input source code must be specified.",
                fullCommand
            );
        }




        // Use default type and path if not specified
        //! Outpuut path is tied to the output type option
        if(r.outputType == '\0') {
            r.outputFile = r.sourceFile + ".out";
            r.outputType = 'x';
        }




        // Use default target platform if not specified
        if(r.targetPlatform == '\0') {
            r.targetPlatform = 'l';
        }




        return r;
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
        std::cout <<
            ansi::bold_magenta << "Command syntax: " << ansi::reset << "alc <source> <options...>\n" <<
            ansi::bold_magenta << "          source │ " << ansi::reset << "The path to the file containing the source code\n" <<
            ansi::bold_magenta << "                 │ \n" <<
            ansi::bold_magenta << "          --help │ " << ansi::reset << "Print this message and ignore any other option but --no-color\n" <<
            ansi::bold_magenta << "       --version │ " << ansi::reset << "Print the version and ignore any other option but --no-color or --help\n" <<
            ansi::bold_magenta << "                 │ \n" <<
            ansi::bold_magenta << "      --no-color │ " << ansi::reset << "Disable colors in the compiler's output\n" <<
            ansi::bold_magenta << "    --no-display │ " << ansi::reset << "Disable source code display in error messages\n" <<
            ansi::bold_magenta << "     --no-errors │ " << ansi::reset << "Don't print error messages (doesn't affect exit code)\n" <<
            ansi::bold_magenta << "     --no-status │ " << ansi::reset << "Disable real-time status and progress bar in the compiler's output\n" <<
            ansi::bold_magenta << "        --silent │ " << ansi::reset << "Only print error messages without showing any additional information            " << ansi::black << "[activates --no-status]\n" << ansi::reset <<
            ansi::bold_magenta << "                 │ \n" <<
            ansi::bold_magenta << "       -p <path> │ " << ansi::reset << "Preprocess the file <source> (without compiling it), write the output in <path> " << ansi::black << "[incompatible with -m and -x]\n" << ansi::reset <<
            ansi::bold_magenta << "       -m <path> │ " << ansi::reset << "Compile the file <source> into a module <path> (without creating an executable) " << ansi::black << "[incompatible with -p and -x]\n" << ansi::reset <<
            ansi::bold_magenta << "       -x <path> │ " << ansi::reset << "Compile the file <source> into an executable file <path>                        " << ansi::black << "[incompatible with -p and -m] "  << ansi::reset << ansi::fill_magenta << " DEFAULT " << ansi::reset << "\n" <<
            ansi::bold_magenta << "                 │ \n" <<
            ansi::bold_magenta << "              -w │ " << ansi::reset << "Compile for Windows systems                                                     " << ansi::black << "[requires -x, incompatible with -w]\n" << ansi::reset <<
            ansi::bold_magenta << "              -l │ " << ansi::reset << "Compile for Linux systems                                                       " << ansi::black << "[requires -x, incompatible with -l] "  << ansi::reset << ansi::fill_magenta << " DEFAULT " << ansi::reset << "\n" <<
            ansi::bold_magenta << "                 │ \n" <<
            ansi::bold_magenta << "       -i <path> │ " << ansi::reset << "Use import path <path> (does not affect include paths)\n" <<
            ansi::bold_magenta << "       -I <path> │ " << ansi::reset << "Use include path <path> (does not affect import paths)\n" <<
            ansi::bold_magenta << "                 │ \n" <<
            ansi::bold_magenta << "        --o-none │ " << ansi::reset << "Turn off all optimizations                                                      " << ansi::black << "[incompatible with other --o options]\n" << ansi::reset <<
            ansi::bold_magenta << "      --o-unused │ " << ansi::reset << "Allow removing unused code                                                      " << ansi::black << "[incompatible with --o-none]\n" << ansi::reset <<
            ansi::bold_magenta << "     --o-rewrite │ " << ansi::reset << "Allow rewriting code to optimize it                                             " << ansi::black << "[incompatible with --o-none]\n" << ansi::reset <<
            ansi::bold_magenta << "      --o-thread │ " << ansi::reset << "Use thread pool                                                                 " << ansi::black << "[incompatible with --o-none]\n" << ansi::reset <<
            ansi::bold_magenta << "      --o-memory │ " << ansi::reset << "Use memory pool                                                                 " << ansi::black << "[incompatible with --o-none]\n" << ansi::reset <<
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