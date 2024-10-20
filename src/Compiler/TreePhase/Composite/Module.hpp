#pragma once
#include "Compiler/TreePhase/Composite/__base_Pattern_Composite.hpp"







namespace cmp {
    //TODO rename to "root"
    struct Pattern_Elm_Module : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;

        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}