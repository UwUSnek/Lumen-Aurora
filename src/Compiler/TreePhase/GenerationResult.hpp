#pragma once
#include <vector>
#include "SourceTree.hpp"




namespace cmp {
    struct GenerationResult {
        std::vector<__base_ST*> trees;
        bool isComplete;
    };
}