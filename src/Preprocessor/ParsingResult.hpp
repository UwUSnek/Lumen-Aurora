#pragma once
#include <string>
#include "StructuredSource.hpp"



namespace pre {
    struct ParsingResult {
        std::string trueValue;      // The original string value
        std::string finalValue;     // The string value after removing line continuation tokens
        ulong height;               // The number of lines the parsed element takes up
        SourceElmType elmType; // The type of element. Set by caller function

        ParsingResult() : ParsingResult(SourceElmType::NONE) {}
        ParsingResult(SourceElmType _elmType) :
            trueValue(""),
            finalValue(""),
            height(0),
            elmType(_elmType) {
        }

        ParsingResult& operator=(const ParsingResult& p) {
            trueValue = p.trueValue;
            finalValue = p.finalValue;
            height = p.height;
            elmType = p.elmType;
            return *this;
        }
    };
}