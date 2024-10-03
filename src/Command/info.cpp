#include <sstream>

#include "info.hpp"
#include "Utils/ansi.hpp"
#include "Command/command.hpp"

#include<iostream>






//TODO update compiler executable name
std::string cmd::getHelpMessage(){
    std::string none______        = ansi::bright_black + " │ ";
    std::string default1_         = ansi::reset + ansi::fill_magenta + ""         + (cmd::options.printColor ? " " : "█") + ansi::reset + " ";
    std::string default__________ = ansi::reset + ansi::fill_magenta + " DEFAULT" + (cmd::options.printColor ? " " : "█") + ansi::reset + " ";


    std::stringstream r;
    r <<
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
    return r.str();
}








std::string cmd::getVersionMessage() {
    std::stringstream r;
    //FIXME print version. get version number dynamically
    r
        << ansi::bold_magenta << "ALC" << ansi::reset << " (Aurora/Lumen Compiler)\n"
        << ansi::bold_magenta << "Version:" << ansi::reset << " L-0.1.0\n"
        // << ansi::bold_magenta << << "Version:" << ansi::reset << " W-0.1.0" //TODO windows build
        // << ansi::bold_magenta << << "Version:" << ansi::reset << " X-0.1.0" //TODO mac build (prob not gonna happen)
    ;
    return r.str();
}