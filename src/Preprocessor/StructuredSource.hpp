#pragma once
#include <string>
#include <string>
#include <vector>



namespace pre {
    enum class SourceElmType {
        DIRECTIVE,
        STRING,
        CHAR,
        COMMENT,
        CODE
    };


    struct SourceElmMeta {
        ulong start;
        std::string file; //TODO set this from the parser function

        SourceElmMeta() = delete;
        SourceElmMeta(ulong _start, std::string _file) :
            start(_start),
            file(_file) {
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
            std::string r = "";
            for(SourceElm elm : elms) {
                r += elm.s;
            }
            return r;
        }
    };
}