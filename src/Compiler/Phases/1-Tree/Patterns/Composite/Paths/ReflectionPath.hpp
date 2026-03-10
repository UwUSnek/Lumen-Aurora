#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"
#include <cstddef>








namespace cmp {
    struct ST_Path_Reflection : public virtual __base_ST {
        ptr<__base_ST> target;
        ptr<__base_ST> value;
        std::string getCategoryName(bool plural = false) const override;
    };
    //TODO check if this works with nested reflection? n:type:max




    struct Pattern_Path_Reflection : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}