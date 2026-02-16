#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Main/ALC.hpp"
#include "compiler.hpp"
#include "Compiler/Phases/0-Tokenization/tokenizationPhase.hpp"
#include "Compiler/Phases/1-Tree/treePhase.hpp"
#include "Utils/ptr.hpp"
#include <memory>




//FIXME make this a command line option
// The size of the token buffer
#define COMPILER_BUFFER_SIZE_MB 5000
#define COMPILER_BUFFER_SIZE ((COMPILER_BUFFER_SIZE_MB * 1000UL * 1000UL) / sizeof(cmp::Token))




ptr<cmp::SourceTree> cmp::compilePreprocessedSourceCode(ptr<pre::AnnotatedSource<false>> b) { //TODO fix return type and value

    // Create subphase buffers
    auto r1 = newptr<TokenizedSource<false>>(COMPILER_BUFFER_SIZE);
    auto r2 = newptr<SourceTree>();

    // Start subphases
    startSubphaseAsync(PhaseID::C0_Tokenization, true, startTokenizationPhase, b, r1);
    // r1->awaitClose(mainCheckErrors); //TODO remove
    startSubphaseAsync(PhaseID::C1_TreeCreation, true, startTreePhase,        r1, r2);

    return r2;
}