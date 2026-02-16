#pragma once
#include "Options.hpp"






namespace cmd {
    extern cmd::Options options;

    void parseOptions(int argc, char* argv[], const std::string &DBG_fullCommand);
}