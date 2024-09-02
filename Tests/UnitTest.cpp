#include <fstream>
#include <iostream>
#include <regex>

#include "UnitTest.hpp"
#include "Utils/ansi.hpp"
#include "Utils/utils.hpp"



std::string compilerLocation = "./Build/out";
std::string tmpDirLocation   = "/tmp/Lumina-Aurora_UnitTests";
std::string tmpFileLocatiton = tmpDirLocation + "/src";
std::string tmpOutputLocatiton = tmpDirLocation + "/out";

ulong totalTests; // Calculated after all tests are added to the vector
ulong passedTests = 0;
ulong failedTests = 0;
ulong activeTests = 0;

std::vector<UnitTest*> tests;







int compile(std::string options, ulong testIndex) {
    return WEXITSTATUS(system((
        compilerLocation + " " + tmpFileLocatiton + std::to_string(testIndex) + " " + options +
        "-I ./Tests"
        " > /dev/null 2> /dev/null"
    ).c_str()));
}




void writeTmpFile(std::string code, ulong testIndex) {
    std::ofstream f(tmpFileLocatiton + std::to_string(testIndex));
    if(!f.is_open()) {
        std::cout << "Temporary file could not be opened";
        exit(1);
    }
    else {
        f << code;
        f.close();
    }
}





void UnitTest::startTest() {
    ++activeTests;
    writeTmpFile(code, testIndex);
}


static void printSeparator(){
    int width = utils::getConsoleWidth();
    std::cout << "\n\n" << ansi::bright_black;
    if(width != -1) {
        for(int i = 0; i < width; ++i) {
            std::cout << "─";
        }
    }
    else {
        std::cout << "────────";
    }
    std::cout << ansi::reset << "\n\n";
}


void UnitTest::checkResult() {
    if(result.peek() != std::stringstream::traits_type::eof()) {
        if(!failedTests) printSeparator();
        ++failedTests;
        std::cout << ansi::bold_red << "Test #" << testIndex << " \"" << ansi::reset << name << ansi::bold_red << "\" failed:\n";
        std::cout << result.str();
        printSeparator();
    }
    else {
        ++passedTests;
    }
    --activeTests;
}




void TestExitValue::startTest() {
    UnitTest::startTest();
    //FIXME USE FULL COMPILATION WHEN AVAILABLE INSTEAD OF JUST -p OR -m
    //FIXME USE FULL COMPILATION WHEN AVAILABLE INSTEAD OF JUST -p OR -m
    //FIXME USE FULL COMPILATION WHEN AVAILABLE INSTEAD OF JUST -p OR -m
    int exitValue = compile(options + "-p " + tmpOutputLocatiton, testIndex);

    if(exitValue != expected) {
        result << ansi::bold_red << "    Expected exit code │ " << ansi::reset << expected << "\n";
        result << ansi::bold_red << "    Actual exit code   │ " << ansi::reset << exitValue ;
    }

    UnitTest::checkResult();
}




void TestPreprocessorOutput::startTest() {
    UnitTest::startTest();
    int exitValue = compile(options + "-p " + tmpOutputLocatiton + std::to_string(testIndex), testIndex);

    if(exitValue) {
        result << ansi::bold_red << "    Compilation stopped with exit code " << exitValue << ".";
    }
    else {
        std::ifstream f(tmpOutputLocatiton + std::to_string(testIndex));
        if(f.is_open()) {

            // Read output file and compare it to the expected output
            std::string outputCode = utils::readFile(f);
            if(outputCode != expected) {
                result << ansi::bold_red << "    Output code doesn't match expected value.\n" << ansi::reset;

                // Print formatted input code
                result << ansi::bold_red << "    Used input:\n        " << ansi::reset;
                for(int i = 0; i < code.length(); ++i) {
                    result << utils::formatChar(code[i], true);
                    if(code[i] == '\n' && i < code.length() - 1) result << "\n        ";
                }

                // Print formatted output code
                result << ansi::bold_red << "\n    Generated output:\n        " << ansi::reset;
                for(int i = 0; i < outputCode.length(); ++i) {
                    result << utils::formatChar(outputCode[i], true);
                    if(outputCode[i] == '\n' && i < outputCode.length() - 1) result << "\n        ";
                }
            }
            f.close();
        }
        else {
            result << ansi::bold_red << "    Output file not found.";
        }
    }

    UnitTest::checkResult();
}