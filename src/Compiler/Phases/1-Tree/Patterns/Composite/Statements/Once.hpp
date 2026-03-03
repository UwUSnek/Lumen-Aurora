#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"
#include <cstddef>








namespace cmp {
    struct ST_Sttm_Once : public virtual __base_ST {
        ptr<__base_ST> statement;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Sttm_Once : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}