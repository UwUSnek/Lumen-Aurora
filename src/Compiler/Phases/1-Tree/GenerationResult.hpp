#pragma once
#include <type_traits>
#include <vector>
#include "SourceTree.hpp"




namespace cmp {
    struct TreeGenerationResult {
        std::vector<ptr<__base_ST>> trees;
        bool isComplete;


        TreeGenerationResult(const std::vector<ptr<__base_ST>> &_trees, bool _isComplete) :
            trees(_trees),
            isComplete(_isComplete) {
        }


        TreeGenerationResult(const int unused [[maybe_unused]], bool _isComplete) :
            TreeGenerationResult(std::vector<ptr<__base_ST>>(), _isComplete) {
        }


        ~TreeGenerationResult() = default;
    };
}