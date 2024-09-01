#pragma once
#include "Options.hpp"






namespace cmd {
    extern cmd::Options options;

    void parseOptions(int argc, char* argv[], std::string DBG_fullCommand);
    void printHelp();
    void printVersion();
}