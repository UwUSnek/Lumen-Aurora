// g++ -std=c++20 -I ./src ./Tests/tests.cpp -o ./Tests/tests.out

#include <string>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "Preprocessor/ErrorCode.hpp"
#include "Utils/ansi.hpp"


std::string compilerLocation = "./Build/out";
std::string tmpDirLocation   = "/tmp/Lumina-Aurora_UnitTests";
std::string tmpFileLocatiton = tmpDirLocation + "/tmp ";

ulong passedTests = 0;
ulong currentTestNumber = 0;






int compile(std::string options) {
    return system((compilerLocation + " " + tmpFileLocatiton + " " + options + " > /dev/null").c_str());
    //TODO update command after implementing real options
}




void writeTmpFile(std::string code) {
    std::filesystem::create_directory(tmpDirLocation);
    std::ofstream f(tmpFileLocatiton);
    if(!f.is_open()) {
        std::cout << "Temporary file could not be opened";
        exit(1);
    }
    else {
        f << code;
        f.close();
    }
}




void testExitValue(std::string testName, ErrorCode expected, std::string code, std::string options = "") {
    ++currentTestNumber;
    writeTmpFile(code);
    int exitValue = compile(options);

    if(exitValue != expected) {
        std::cout << ansi::bold_red << "Test #" << currentTestNumber << " \"" << ansi::reset << testName << ansi::bold_red << "\" failed:\n";
        std::cout << ansi::bold_red << "    Expected exit code │ " << ansi::reset << expected << "\n";
        std::cout << ansi::bold_red << "    Actual exit code   │ " << ansi::reset << exitValue << "\n";
    }
    else {
        ++passedTests;
    }
}
// void testFileOutput()




int main(){
    std::cout << "Running tests...\n\n";


    testExitValue("idk", ErrorCode::ERROR_CHAR_INCOMPLETE_0, "test");


    std::cout << (passedTests == currentTestNumber ? ansi::bold_green : ansi::bold_red);
    std::cout << "\n\nTests passed: " << passedTests << "/" << currentTestNumber << "\n" << ansi::reset;
    return 0;
}