#include "ElmCoords.hpp"
#include "StructuredSource.hpp"




ElmCoords::ElmCoords(pre::SourceElmMeta &meta) :
    ElmCoords(meta.file, meta.lineNum, meta.start, meta.start + meta.trueValue.length()) {
}