#include "ALC.hpp"
#include "compiler.hpp"
#include "TokenizationPhase/tokenizationPhase.hpp"
#include "TreePhase/treePhase.hpp"








void cmp::compilePreprocessedSourceCode(pre::SegmentedCleanSource* b) {

    // Create subphase buffers
    TokenizedSource *r1 = new TokenizedSource();
    ST_Module       *r2 = new ST_Module();

    // Start subphases
    startSubphaseAsync(Compilation, false, startTokenizationPhase, b, r1);
    startSubphaseAsync(Compilation, false, startTreePhase,        r1, r2);
}