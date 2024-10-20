#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Compiler/TreePhase/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_Struct : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        //! Enum elements are saved in the children vector

        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Struct : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}