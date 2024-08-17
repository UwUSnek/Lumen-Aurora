#pragma once
#include "Preprocessor/CleanupPhase/IntermediateCodeFormat.hpp"



namespace pre {
    IntermediateCodeFormat startDirectivesPhase(IntermediateCodeFormat cleanCode, std::string DEBUG_filePath);

    // ParsingResult parseDirective(std::string b, ulong index, ulong DEBUG_curLine, std::string DEBUG_filePath);
}