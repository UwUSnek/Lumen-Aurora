#! /bin/bash



mkdir -p ./Build
cmake -B ./Build -S ./Tools

cd ./Build
make -j8

#TODO add a separate build for the optimized version, maybe wait til rewrite