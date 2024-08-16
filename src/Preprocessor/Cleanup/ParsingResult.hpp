#pragma once
#include <string>
#include "ICF_Clean.hpp"



namespace pre {
    struct ParsingResult {
        std::string trueValue;      // The original string value
        std::string finalValue;     // The string value after removing line continuation tokens
        ulong height;               // The number of lines the parsed element takes up
        IFC_CleanType elmType; // The type of element. Set by caller function

        ParsingResult() : ParsingResult(IFC_CleanType::NONE) {}
        ParsingResult(IFC_CleanType _elmType) :
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