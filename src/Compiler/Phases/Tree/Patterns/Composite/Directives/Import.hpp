#pragma once
#include "Compiler/Phases/Tree/SourceTree.hpp"
#include "Compiler/Phases/Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct __internal_ST_Import_Elm : public virtual __base_ST_Referable {
        ST_Sub_Path *symbol;
        explicit __internal_ST_Import_Elm(ST_Sub_Path *_symbol) : symbol(_symbol){}
    };

    struct ST_Import : public virtual __base_ST_Referable {
        std::string rawPath;
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