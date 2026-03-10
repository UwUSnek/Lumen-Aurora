#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_Sttm_Break : public virtual __base_ST {

        //! nullptr if not specified (equivalent to 1)
        ptr<__base_ST> amount;

        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Sttm_Break : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}