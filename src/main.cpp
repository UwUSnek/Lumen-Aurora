#include <iostream>
#include <vector>
#include <string>
#include <fstream>

#include "Options.hpp"
#include "Preprocessor/Preprocessor.hpp"




Options parseOptions(int argc, char* argv[]){
    Options r;
    for(int i = 1; i < argc; ++i) {
        r.sourceFile = std::string(argv[i]);
    }
    return r;
}



int main(int argc, char* argv[]){
    Options options = parseOptions(argc, argv);

    std::string sourceFile = pre::loadSourceFile(options);
    std::string sourceCode = pre::processIncludes(sourceFile);
    std::cout << sourceCode;
    //std::string fileBuffer =
}