#! /bin/bash




# Generate build number
mkdir ./src/Generated
> ./src/Generated/buildNumber.hpp
    echo "//!                  DO NOT .GITIGNORE THIS HEADER"         >> ./src/Generated/buildNumber.hpp
    echo "//!          THE BUILD NUMBER IS USED TO TELL BUILDS APART" >> ./src/Generated/buildNumber.hpp
    echo ""                                                           >> ./src/Generated/buildNumber.hpp
    echo "#pragma once"                                               >> ./src/Generated/buildNumber.hpp
    echo "#include <string>"                                          >> ./src/Generated/buildNumber.hpp
    echo ""                                                           >> ./src/Generated/buildNumber.hpp
    echo "static const std::string buildNumber = \"$(uuidgen)\";"     >> ./src/Generated/buildNumber.hpp




# Create build directory and run cmake
mkdir -p ./Build
cmake -B ./Build -S ./Tools

# Build the source code (8 threads)
cd ./Build
make -j8

#TODO add a separate build for the optimized version, maybe wait til rewrite