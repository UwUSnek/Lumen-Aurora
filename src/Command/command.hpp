#pragma once
#include "Options.hpp"






namespace cmd {
    extern cmd::Options options;

    void parseOptions(int argc, char* argv[]);
    void printHelp();
    void printVersion();
}