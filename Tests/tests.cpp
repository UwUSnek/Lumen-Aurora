// g++ -std=c++20 -I ./src ./Tests/tests.cpp ./Tests/UnitTest.cpp ./src/Utils/utils.cpp -o ./Tests/tests.out

#include <string>
#include <vector>
#include <iostream>
#include <filesystem>
#include "ErrorCode.hpp"
#include "Utils/ansi.hpp"
#include "UnitTest.hpp"


// void testFileOutput()



//FIXME show a progressbar that indicates (with colors) what percentage of tests have passed, failed or are currently being executed
//FIXME use a different tmp file for each thread


int main(){

    std::filesystem::create_directory(tmpDirLocation);
    std::cout << "Running tests...\n\n";




    tests.push_back(new TestExitValue("string \\0", "\"test",     ErrorCode::ERROR_STRING_INCOMPLETE_0));
    tests.push_back(new TestExitValue("string \\n", "\"test\n\"", ErrorCode::ERROR_STRING_INCOMPLETE_n));

    tests.push_back(new TestExitValue("char \\0",   "'test",      ErrorCode::ERROR_CHAR_INCOMPLETE_0));
    tests.push_back(new TestExitValue("char \\n",   "'test\n'",   ErrorCode::ERROR_CHAR_INCOMPLETE_n));


    tests.push_back(new TestPreprocessorOutput(
        "empty output",
        "",
        ""
    ));
    tests.push_back(new TestPreprocessorOutput(
        "line merge",
        "abc\\\nabc",
        "abcabc"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "line merge \\0",
        "abc\\",
        "abc"
    ));


    tests.push_back(new TestPreprocessorOutput(
        "comment removal 1",
        "abc/*comment*/abc//test",
        "abcabc"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "comment removal 2",
        "abc/*comment*/abc//test\n",
        "abcabc\n"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "comment removal 3",
        "abc/*com\n\nm\ne\\\nnt*//*t//est*/abc",
        "abcabc"
    ));


    totalTests = tests.size();
    for(ulong i = 0; i < tests.size(); ++i) {
        //TODO make this multithreaded
        tests[i]->startTest();
    }




    std::cout << "\n\n";
    std::cout << (passedTests == totalTests ? ansi::bold_green : ansi::reset) << "Passed: " << passedTests << "/" << totalTests << "\n";
    std::cout << (failedTests > 0           ? ansi::bold_red   : ansi::reset) << "Failed: " << failedTests << "/" << totalTests << "\n";
    std::cout << ansi::reset;
    return 0;
}