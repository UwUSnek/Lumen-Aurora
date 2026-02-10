#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"







namespace cmp {
    struct ST_Type : public virtual __base_ST {
        __base_ST *type = nullptr;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Type : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong   getCertaintyThreshold() const override;
    };
}