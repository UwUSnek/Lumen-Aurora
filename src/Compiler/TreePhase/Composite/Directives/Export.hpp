#pragma once
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Compiler/TreePhase/Composite/__base_Pattern_Composite.hpp"







namespace cmp {
    struct __internal_ST_Export_Elm {
        ST_Sub_Path *symbol;
        std::string name;
        explicit __internal_ST_Export_Elm(ST_Sub_Path *_symbol) : symbol(_symbol){}
    };

    struct ST_Export : public virtual __base_ST_Referable {
        std::vector<__internal_ST_Export_Elm> elms;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Export : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}