#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




struct ElmCoords {
    ulong filePathIndex;
    ulong lineNum;
    ulong start;
    ulong end;
    bool overflow;

    ElmCoords() :
        filePathIndex(-1),
        lineNum(0),
        start(0),
        end(0),
        overflow(false) {
    }

    ElmCoords(ulong _filePathIndex, ulong _lineNum, ulong _start, ulong _end) :
        filePathIndex(_filePathIndex),
        lineNum(_lineNum),
        start(_start),
        end(_end),
        overflow(false) { //TODO idk if this is false correct
    }

    ElmCoords(ulong _filePathIndex, ulong _lineNum, ulong _start, ulong _end, bool _overflow) :
        filePathIndex(_filePathIndex),
        lineNum(_lineNum),
        start(_start),
        end(_end),
        overflow(_overflow) {
    }




    template<bool sr> ElmCoords(ptr<pre::AnnotatedSource<sr>> source, ulong _start, ulong _end) : ElmCoords(*source, _start, _end) {};
    template<bool sr> ElmCoords(ptr<cmp::TokenizedSource<sr>> source, ulong _start, ulong _end) : ElmCoords(*source, _start, _end) {};




    template<bool sr> ElmCoords(pre::AnnotatedSource<sr> &source, ulong _start, ulong _end) {
        if(source.length() == 0) {
            filePathIndex = 0;
            lineNum       = 0;
            start         = 0;
            end           = 0;
        }
        else {
            overflow = _end >= source.length();
            ulong actual_start = _start >= source.length() ? source.length() - 1 : _start;
            ulong actual_end   = _end   >= source.length() ? source.length() - 1 : _end;

            filePathIndex = source[actual_start]->meta.f;
            lineNum       = source[actual_start]->meta.l;
            start         = source[actual_start]->meta.i;
            end           = source[actual_end  ]->meta.i;
        }
    }




    template<bool sr> ElmCoords(cmp::TokenizedSource<sr> &source, ulong _start, ulong _end) {
        if(source.length() == 0) {
            filePathIndex = 0;
            lineNum       = 0;
            start         = 0;
            end           = 0;
        }
        else {
            overflow = _end >= source.length();
            ulong actual_start = _start >= source.length() ? source.length() - 1 : _start;
            ulong actual_end   = _end   >= source.length() ? source.length() - 1 : _end;

            filePathIndex = source[actual_start]->start.f;
            lineNum       = source[actual_start]->start.l;
            start         = source[actual_start]->start.i;
            end           = source[actual_end  ]->end  .i;
        }
    }
};