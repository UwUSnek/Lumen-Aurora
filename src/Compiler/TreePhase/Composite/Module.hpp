#pragma once
#include "Compiler/TreePhase/Composite/__base_Pattern_Composite.hpp"







namespace cmp {
    //TODO rename to "root"
    struct Pattern_Elm_Module : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;

        std::string genDecoratedValue() const override { return "" debug("Debug:Module"); }
        ulong getCertaintyThreshold() const override;
        // virtual bool isChildAllowed(__base_ST* const c) const {
            // return !c->isStatement();
        // }
    };
}