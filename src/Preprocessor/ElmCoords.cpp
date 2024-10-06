#include "ElmCoords.hpp"
#include "Preprocessor/SegmentedCleanSource.hpp"
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"



//FIXME SHOW ALL THE FILES A SEGMENT SPANS ACROSS, IF THEY ARE MORE THAN ONE
// FIXME prob will need to save the end's path and line as well


ElmCoords::ElmCoords(pre::SegmentedCleanSource *source, ulong _start, ulong _end) :
    filePathIndex(source->meta[_start]->f),
    lineNum      (source->meta[_start]->l),
    start        (source->meta[_start]->i),
    end          (source->meta[_end  ]->i) {
}


ElmCoords::ElmCoords(cmp::TokenizedSource *source, ulong _start, ulong _end) :
    filePathIndex((*source)[_start]->start.f),
    lineNum      ((*source)[_start]->start.l),
    start        ((*source)[_start]->start.i),
    end          ((*source)[_end  ]->end  .i) {
}