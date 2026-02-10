#include "ElmCoords.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"



//FIXME SHOW ALL THE FILES A SEGMENT SPANS ACROSS, IF THEY ARE MORE THAN ONE
// FIXME prob will need to save the end's path and line as well

//FIXME MOVE ALL DEFINITIONS TO CPP FILES
//FIXME MOVE ALL DEFINITIONS TO CPP FILES
//FIXME MOVE ALL DEFINITIONS TO CPP FILES
//FIXME MOVE ALL DEFINITIONS TO CPP FILES




ElmCoords::ElmCoords(ptr<pre::SegmentedCleanSource> source, ulong _start, ulong _end) {
    if(source->meta.length() == 0) {
        filePathIndex = 0;
        lineNum       = 0;
        start         = 0;
        end           = 0;
    }
    else {
        overflow = _end >= source->meta.length();
        ulong actual_start = _start > source->str.length() ? source->meta.length() - 1 : _start;
        ulong actual_end   = _end   > source->str.length() ? source->meta.length() - 1 : _end;

        filePathIndex = source->meta[actual_start]->f;
        lineNum       = source->meta[actual_start]->l;
        start         = source->meta[actual_start]->i;
        end           = source->meta[actual_end]->i;
    }
}


ElmCoords::ElmCoords(ptr<cmp::TokenizedSource> source, ulong _start, ulong _end) {
    if(source->length() == 0) {
        filePathIndex = 0;
        lineNum       = 0;
        start         = 0;
        end           = 0;
    }
    else {
        overflow = _end >= source->length();
        ulong actual_start = _start > source->length() ? source->length() - 1 : _start;
        ulong actual_end   = _end   > source->length() ? source->length() - 1 : _end;

        filePathIndex = (*source)[actual_start]->start.f;
        lineNum       = (*source)[actual_start]->start.l;
        start         = (*source)[actual_start]->start.i;
        end           = (*source)[actual_end  ]->end  .i;
    }
}