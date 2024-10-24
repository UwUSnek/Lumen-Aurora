#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Compiler/TreePhase/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct __internal_ST_Import_Elm {
        ST_Sub_Path *symbol;
        std::string name;
        __internal_ST_Import_Elm(ST_Sub_Path *_symbol) : symbol(_symbol){}
    };

    struct ST_Import : public virtual __base_ST_Referable {
        std::string name;
        std::vector<__internal_ST_Import_Elm> elms;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Import : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}