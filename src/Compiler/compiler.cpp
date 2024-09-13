#include "ALC.hpp"
#include "compiler.hpp"
#include "TokenizationPhase/tokenizationPhase.hpp"


void cmp::compilePreprocessedSourceCode(pre::SegmentedCleanSource* b) {
    TokenizedSource *r1 = new TokenizedSource();

    startSubphaseAsync(PhaseID::COMPILATION, startTokenizationPhase, b, r1);
}