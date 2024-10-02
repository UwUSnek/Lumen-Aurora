#include "ALC.hpp"
#include "compiler.hpp"
#include "TokenizationPhase/tokenizationPhase.hpp"
#include "TreePhase/treePhase.hpp"








void cmp::compilePreprocessedSourceCode(pre::SegmentedCleanSource* b) {

    // Set max progress (updated dynamically by preprocessor threads whenever they remove stuff after merging the files) //TODO remove comment
    // increaseMaxProgress(fetchMaxProgress(Preprocessing) * 2); //FIXME use *n for n subphases //TODO remove comment
    // increaseMaxProgress(Compilation, fetchMaxProgress(Preprocessing) * 1); //TODO remove comment
    //! Max progress is calculted and set by preprocessor subpahses

    // Create subphase buffers
    TokenizedSource *r1 = new TokenizedSource();
    ST_Module       *r2 = new ST_Module();

    // Start subphases
    startSubphaseAsync(Compilation, false, startTokenizationPhase, b, r1);
    startSubphaseAsync(Compilation, false, startTreePhase,        r1, r2);
}