// g++ -std=c++20 -I ./src ./Tests/tests.cpp -o ./Tests/tests.out

#include <string>
#include <iostream>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include "ErrorCode.hpp"
#include "Utils/ansi.hpp"


std::string compilerLocation = "./Build/out";
std::string tmpDirLocation   = "/tmp/Lumina-Aurora_UnitTests";
std::string tmpFileLocatiton = tmpDirLocation + "/tmp";

ulong passedTests = 0;
ulong currentTestNumber = 0;






int compile(std::string options) {
    return WEXITSTATUS(system((compilerLocation + " " + tmpFileLocatiton + " " + options + " > /dev/null 2> /dev/null").c_str()));
    //TODO update command after implementing real options
}




void writeTmpFile(std::string code) {
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
        std::cout << ansi::bold_red << "\nTest #" << currentTestNumber << " \"" << ansi::reset << testName << ansi::bold_red << "\" failed:\n";
        std::cout << ansi::bold_red << "    Expected exit code │ " << ansi::reset << expected << "\n";
        std::cout << ansi::bold_red << "    Actual exit code   │ " << ansi::reset << exitValue << "\n";
    }
    else {
        ++passedTests;
    }
}
// void testFileOutput()



//FIXME replace test functions with 1 test function that takes an object
//FIXME the object takes the old parameters as constructor parameters and is pushed into a vector
//FIXME the tests in the vector are then counted and split between the threads
//FIXME show a progressbar that indicates (with colors) what percentage of tests have passed, failed or are currently being executed

//FIXME use a different tmp file for each thread


int main(){
    std::filesystem::create_directory(tmpDirLocation);
    std::cout << "Running tests...\n\n";


    testExitValue("string \\0", ErrorCode::ERROR_STRING_INCOMPLETE_0, "\"test");
    testExitValue("string \\n", ErrorCode::ERROR_STRING_INCOMPLETE_n, "\"test\n\"");

    testExitValue("char \\0", ErrorCode::ERROR_CHAR_INCOMPLETE_0,     "'test");
    testExitValue("char \\n", ErrorCode::ERROR_CHAR_INCOMPLETE_n,     "'test\n'");
//BUG fix incorrect exit code

    std::cout << (passedTests == currentTestNumber ? ansi::bold_green : ansi::bold_red);
    std::cout << "\n\nTests passed: " << passedTests << "/" << currentTestNumber << "\n" << ansi::reset;
    return 0;
}