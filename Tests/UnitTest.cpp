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





void UnitTest::startTest() {
    ++activeTests;
    writeTmpFile(code);
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
    int exitValue = compile(options);

    if(exitValue != expected) {
        result << ansi::bold_red << "    Expected exit code │ " << ansi::reset << expected << "\n";
        result << ansi::bold_red << "    Actual exit code   │ " << ansi::reset << exitValue ;
    }

    UnitTest::checkResult();
}




void TestPreprocessorOutput::startTest() {
    UnitTest::startTest();
    int exitValue = compile(options);

    if(exitValue) {
        result << ansi::bold_red << "    Compilation stopped with exit code " << exitValue << ".";
    }
    else {
        //FIXME ADD OUTPUT OPTION TO COMPILER
        //FIXME ADD OUTPUT OPTION TO COMPILER
        //FIXME ADD OUTPUT OPTION TO COMPILER
        //FIXME ADD OUTPUT OPTION TO COMPILER
        //FIXME ADD OUTPUT OPTION TO COMPILER
        std::ifstream f(tmpOutputLocatiton);
        if(f.is_open()) {
            std::string outputCode = utils::readFile(f);
            if(outputCode != expected) {
                result << ansi::bold_red << "    Output code doesn't match.\n    " << ansi::reset;
                for(int i = 0; i < outputCode.length(); ++i) {
                    result << utils::formatChar(outputCode[i], true);
                    if(outputCode[i] == '\n' && i < outputCode.length() - 1) result << "\n    ";
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