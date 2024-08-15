#pragma once


namespace pre {
    class WhitespaceInfo {
    public:
        unsigned long w;
        unsigned long h;
        bool isBreaking;

        WhitespaceInfo() :
            w(0),
            h(1),
            isBreaking(false) {
        }
    };
}