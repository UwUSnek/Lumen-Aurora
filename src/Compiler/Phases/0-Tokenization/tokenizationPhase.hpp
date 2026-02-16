#pragma once
#include "Preprocessor/AnnotatedSource.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Utils/ptr.hpp"




//TODO extension should have some view layer / keybind to show a box around each token.
//TODO this is to make it easier to read code with dumb comment and LSTs placements.
namespace cmp {
    void __internal_startTokenizationPhase(ptr<pre::AnnotatedSource<false>> b, ptr<TokenizedSource<false>> r);
    void            startTokenizationPhase(ptr<pre::AnnotatedSource<false>> b, ptr<TokenizedSource<false>> r);
}