#pragma once
#include <string>
#include <vector>
#include <regex>
#include <algorithm>

#include "Utils/utils.hpp"




namespace pre {
    enum class SourceElmType {
        NONE,       // Specified element not found
        DIRECTIVE_INCLUDE,
        DIRECTIVE_DEFINE,
        STRING,
        CHAR,
        COMMENT,
        CODE
    };
    static inline std::string sourceElmTypeToString(SourceElmType type) {
        switch(type) {
            case SourceElmType::DIRECTIVE_INCLUDE: return "DIRECTIVE_INCLUDE";
            case SourceElmType::DIRECTIVE_DEFINE: return "DIRECTIVE_DEFINE";
            case SourceElmType::STRING:    return "STRING";
            case SourceElmType::CHAR:      return "CHAR";
            case SourceElmType::COMMENT:   return "COMMENT";
            case SourceElmType::CODE:      return "CODE";
        }
        return "NONE";
    }



    struct SourceElmMeta {
        ulong start;
        std::string file; //TODO set this from the parser function
        ulong lineNum;
        std::string trueValue;

        SourceElmMeta() = delete;
        SourceElmMeta(ulong _start, std::string _file, ulong _lineNum, std::string _trueValue) :
            start(_start),
            file(_file),
            lineNum(_lineNum),
            trueValue(_trueValue) {
        }
    };


    struct SourceElm {
        SourceElmType type;
        std::string s;
        SourceElmMeta meta;

        SourceElm() = delete;
        SourceElm(SourceElmType _type, std::string _s, SourceElmMeta _meta) :
            type(_type),
            s(_s),
            meta(_meta) {
        }
    };




    struct StructuredSource {
        std::vector<SourceElm> elms = std::vector<SourceElm>();

        std::string toString(){
            static ulong elmTypeNameMaxSpace = 32;
            std::string r = "";
            for(SourceElm elm : elms) {
                std::string elmTypeName = sourceElmTypeToString(elm.type);
                r += elmTypeName;
                r.append(std::max(1l, (long)elmTypeNameMaxSpace - (long)elmTypeName.length()), ' ');
                r += "| ";
                r += std::regex_replace(std::regex_replace(elm.s, std::regex("\n"), ansi::black + "\\n" + ansi::reset), std::regex(" "), ansi::black + "·" + ansi::reset);
                r += "\n";
            }
            return r;
        }
    };
}