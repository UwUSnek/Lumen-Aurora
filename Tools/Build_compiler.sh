#! /bin/bash



mkdir -p ./Build
cmake -B ./Build -S ./Tools

cd ./Build
make

#TODO add a separate build for the optimized version, maybe wait til rewrite