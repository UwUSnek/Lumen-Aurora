#pragma once
#include <string>
#include <vector>
#include "ErrorCode.hpp"



extern std::string compilerLocation;
extern std::string tmpDirLocation;
extern std::string tmpFileLocatiton;
extern std::string tmpOutputLocatiton;

extern ulong totalTests;
extern ulong passedTests;
extern ulong failedTests;
extern ulong activeTests;

struct UnitTest;
extern std::vector<UnitTest*> tests;










struct UnitTest {
    std::string name;
    std::string code;
    std::string options;

    int testIndex;
    std::stringstream result;

public:
    UnitTest(std::string _name, std::string _code, std::string _options = "") :
        testIndex(tests.size()),
        name(_name),
        code(_code),
        options(_options) {
    }
    virtual void startTest();
    void checkResult();
};








struct TestExitValue : UnitTest {
    ErrorCode expected;

public:
    TestExitValue(std::string _name, std::string _code, ErrorCode _expected, std::string _options = "") :
        UnitTest(_name, _code, _options),
        expected(_expected) {
    }
    void startTest() override;
};








struct TestPreprocessorOutput : UnitTest {
    std::string expected;

public:
    TestPreprocessorOutput(std::string _name, std::string _code, std::string _expected, std::string _options = "") :
        UnitTest(_name, _code, _options),
        expected(_expected) {
    }
    void startTest() override;
};