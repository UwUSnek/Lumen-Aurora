#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"







namespace cmp {
    struct __internal_ST_Export_Elm : public virtual __base_ST_Referable {
        ST_Sub_Path *symbol;
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