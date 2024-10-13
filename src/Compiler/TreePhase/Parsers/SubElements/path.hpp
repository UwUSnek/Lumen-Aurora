#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"








namespace cmp {
    struct ST_Sub_Path : public virtual __base_ST {
        std::vector<std::string> idList;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Path : public virtual __base_Pattern_Composite {
        Pattern_Elm_Path();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;

        // virtual bool isChildAllowed(__base_ST* const c) const {
            // return !c->isStatement();
        // }
    };
}