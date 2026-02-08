#pragma once
#include <vector>
#include "SourceTree.hpp"




namespace cmp {
    struct TreeGenerationResult {
        std::vector<__base_ST*> trees;
        bool isComplete;

        ~TreeGenerationResult(){};
    };
}