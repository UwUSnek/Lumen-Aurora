#include <string>
#include <vector>
#include "Utils/console.hpp"
#include <filesystem>
namespace fs = std::filesystem;

#include "Main/ErrorCode.hpp"
#include "Utils/ansi.hpp"
#include "UnitTest.hpp"

// void testFileOutput()



//FIXME show a progressbar that indicates (with colors) what percentage of tests have passed, failed or are currently being executed
//FIXME use a different tmp file for each thread


int main(){

    fs::create_directory(tmpDirLocation);
    console::cout << "Running tests...\n\n";




    tests.push_back(new TestExitValue("string \\0", "\"test",     ErrorCode::ERROR_CMP_STRING_INCOMPLETE_0));
    tests.push_back(new TestExitValue("string \\n", "\"test\n\"", ErrorCode::ERROR_CMP_STRING_INCOMPLETE_n));

    tests.push_back(new TestExitValue("char \\0",   "'test",      ErrorCode::ERROR_CMP_CHAR_INCOMPLETE_0));
    tests.push_back(new TestExitValue("char \\n",   "'test\n'",   ErrorCode::ERROR_CMP_CHAR_INCOMPLETE_n));




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




    tests.push_back(new TestPreprocessorOutput(
        "include accuracy 1",
        "A#include  \t\"basic.lmn\"B",
        "AtestB",
        "-I Tests/Includes/Accuracy"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "include accuracy 2",
        "A#include  \t\"inception2.lmn\"B",
        "AtestB",
        "-I Tests/Includes/Accuracy"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "include accuracy - comment only",
        "A#include  \t\"commentOnly.lmn\"B",
        "AB",
        "-I Tests/Includes/Accuracy"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "include accuracy - comment boundaries",
        "A#include  \t\"commentBoundaries.lmn\"B",
        "AtestB",
        "-I Tests/Includes/Accuracy"
    ));



    tests.push_back(new TestExitValue(
        "include conflict",
        "A#include   \t\"test.lmn\"B",
        ErrorCode::ERROR_PRE_PATH_AMBIGUOUS,
        "-I Tests/Includes/Conflict1 -I Tests/Includes/Conflict2"
    ));
    tests.push_back(new TestPreprocessorOutput(
        "include path merging",
        "A#include   \t\"test.lmn\"B",
        "AhelloB",
        "-I Tests/Includes/Conflict1 -I Tests/Includes/Conflict1 -I Tests/Includes/./../Includes/Conflict1"
    ));



    totalTests = tests.size();
    for(ulong i = 0; i < tests.size(); ++i) {
        //TODO make this multithreaded
        tests[i]->startTest();
    }




    console::cout << "\n\n";
    console::cout << (passedTests == totalTests ? ansi::bold_bright_green : ansi::reset) << "Passed: " << passedTests << "/" << totalTests << "\n";
    console::cout << (failedTests > 0           ? ansi::bold_red   : ansi::reset) << "Failed: " << failedTests << "/" << totalTests << "\n";
    console::cout << ansi::reset;
    return 0;
}