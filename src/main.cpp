#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Options.hpp"
#include "Preprocessor/preprocessor.hpp"
#include "Preprocessor/StructuredSource.hpp"




Options parseOptions(int argc, char* argv[]){
    Options r;
    for(int i = 1; i < argc; ++i) {
        r.sourceFile = std::string(argv[i]);
    }
    return r;
}



//TODO run all passes concurrently. make the next pass wait for the previous one when it reached it and make them read from streams
int main(int argc, char* argv[]){
    Options options = parseOptions(argc, argv);

    pre::StructuredSource sourceCode = pre::loadSourceCode(options);
    std::cout << sourceCode.toString();
}