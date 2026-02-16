#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_StructElmCluster : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        std::vector<ptr<ST_StructElm>> elms;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_StructElmCluster : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong   getCertaintyThreshold() const override;
    };
}