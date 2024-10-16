#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"








namespace cmp {
    struct ST_Namespace : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Namespace : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue() const override;
        ulong   getCertaintyThreshold() const override;
        // virtual bool isChildAllowed(__base_ST* const c) const {
        //     return !c->isStatement();
        // }
    };
}