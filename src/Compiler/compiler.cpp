#include "ALC.hpp"
#include "compiler.hpp"
#include "TokenizationPhase/tokenizationPhase.hpp"
#include "TreePhase/treePhase.hpp"








cmp::SourceTree* cmp::compilePreprocessedSourceCode(pre::SegmentedCleanSource* b) { //TODO fix return type and value

    // Set max progress (updated dynamically by preprocessor threads whenever they remove stuff after merging the files) //TODO remove comment
    // increaseMaxProgress(fetchMaxProgress(Preprocessing) * 2); //FIXME use *n for n subphases //TODO remove comment
    // increaseMaxProgress(Compilation, fetchMaxProgress(Preprocessing) * 1); //TODO remove comment
    //! Max progress is calculted and set by preprocessor subpahses

    // Create subphase buffers
    TokenizedSource *r1 = new TokenizedSource();
    SourceTree      *r2 = new SourceTree();

    // Start subphases
    startSubphaseAsync(Compilation, false, startTokenizationPhase, b, r1);
    startSubphaseAsync(Compilation, true, startTreePhase,        r1, r2); //FIXME set islast to false and use true in the subphase that's actually last

    return r2;
}