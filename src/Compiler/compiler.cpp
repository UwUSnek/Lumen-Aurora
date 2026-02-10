#include "Main/ALC.hpp"
#include "compiler.hpp"
#include "Compiler/Phases/0-Tokenization/tokenizationPhase.hpp"
#include "Compiler/Phases/1-Tree/treePhase.hpp"








ptr<cmp::SourceTree> cmp::compilePreprocessedSourceCode(ptr<pre::SegmentedCleanSource> b) { //TODO fix return type and value

    // Create subphase buffers
    auto r1 = newptr<TokenizedSource>();
    auto r2 = newptr<SourceTree>();

    // Start subphases
    startSubphaseAsync(PhaseID::Compiler_Tokenization, true, startTokenizationPhase, b, r1);
    startSubphaseAsync(PhaseID::Compiler_TreeCreation, true, startTreePhase,        r1, r2);

    return r2;
}