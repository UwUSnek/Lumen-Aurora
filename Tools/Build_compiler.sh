#! /bin/bash
original_dir=$(pwd)




# Write build number header
mkdir ./src/Generated
buildNumberPlaceholder="__BUILD_NUMBER_PLACEHOLDER__ __SHA1SUM__" #! Same number of characters as a sha1 output
> ./src/Generated/buildNumber.hpp
    echo "//!                  DO NOT .GITIGNORE THIS HEADER"                  >> ./src/Generated/buildNumber.hpp
    echo "//!          THE BUILD NUMBER IS USED TO TELL BUILDS APART"          >> ./src/Generated/buildNumber.hpp
    echo ""                                                                    >> ./src/Generated/buildNumber.hpp
    echo "#pragma once"                                                        >> ./src/Generated/buildNumber.hpp
    echo "#include <string>"                                                   >> ./src/Generated/buildNumber.hpp
    echo ""                                                                    >> ./src/Generated/buildNumber.hpp
    echo "// The actual value is set after compiling the code"                 >> ./src/Generated/buildNumber.hpp
    echo "static const std::string buildNumber = \"$buildNumberPlaceholder\";" >> ./src/Generated/buildNumber.hpp




# Create build directory and run cmake
mkdir -p ./Build
cmake -B ./Build -S ./Tools

# Build the source code (8 threads)
cd ./Build
make -j8




# Generate actual build number from the output executable's sha1
cd $original_dir                                                                    # Move back to root dir
buildNumber=$(sha1sum ./Build/alc | awk '{ print $1 }')                             # Calculate sha1sum
perl -pe "s/$buildNumberPlaceholder/$buildNumber/g" ./Build/alc > ./Build/alc2      # Replace the placeholder and save output to new file
chmod --reference=./Build/alc ./Build/alc2                                          # Copy permissions of old executable
mv ./Build/alc2 ./Build/alc                                                         # Replace the old executable with the modified one





#TODO add a separate build for the optimized version, maybe wait til rewrite