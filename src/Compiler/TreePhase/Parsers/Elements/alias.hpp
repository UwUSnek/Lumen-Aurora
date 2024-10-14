#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"








namespace cmp {
    struct ST_Alias : public virtual __base_ST_Referable {
        ST_Sub_Path *original = nullptr;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Alias : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;

        // virtual bool isChildAllowed(__base_ST* const c) const {
        //     return !c->isStatement();
        // }
    };
}