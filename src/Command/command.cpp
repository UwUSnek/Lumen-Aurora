#include <iostream>
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
        for(int i = 1; i < argc; ++i) {
            std::string o(argv[i]);


            // Options
            if(o[0] == '-') {
                if(o.length() == 2) switch(o[1]) {
                    case 'p': {

                    }
                    case 'm': {

                    }
                    case 'x': {

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
                        cmd::CommandCoords(0, 0),
                        cmd::CommandCoords(0, 0),
                        "Unknown command line option \"" + ansi::white + o + ansi::reset + "\".",
                        fullCommand
                    );
                }
            }


            // Source file
            else {
                r.sourceFile = o;
            }
        }
        return r;
    }








    //TODO update compiler executable name
    void printHelp(){
        std::cout <<
            ansi::bold_magenta << "Command syntax: " << ansi::reset << "alc <source> [-p|-m|-x] <output> <options>\n" <<
            ansi::bold_magenta << "     source │ " << ansi::reset << "The path to the file containing the source code\n" <<
            ansi::bold_magenta << "     output │ " << ansi::reset << "The path to the file in which to write the specified output (-p, -m or -x)\n" <<
            ansi::bold_magenta << "            │ \n" <<
            ansi::bold_magenta << "     --help │ " << ansi::reset << "Print this message and ignore any other option\n" <<
            ansi::bold_magenta << "  --version │ " << ansi::reset << "Print the version and ignore any other option but --help\n" <<
            ansi::bold_magenta << " --no-color │ " << ansi::reset << "Disable colors in the compiler's output\n" <<
            ansi::bold_magenta << "--no-status │ " << ansi::reset << "Disable real-time status and progress bar in the compiler's output\n" <<
            ansi::bold_magenta << "            │ \n" <<
            ansi::bold_magenta << "         -p │ " << ansi::reset << "Preprocess the source file (without compiling it)\n" <<
            ansi::bold_magenta << "         -m │ " << ansi::reset << "Compile the source file into a module (without creating an executable)\n" <<
            ansi::bold_magenta << "         -x │ " << ansi::reset << "Compile the source file into an executable file\n" <<
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