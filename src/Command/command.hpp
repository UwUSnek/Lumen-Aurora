#pragma once
#include "Options.hpp"



namespace cmd {
    Options parseOptions(int argc, char* argv[]);
    void printHelp();
    void printVersion();
}