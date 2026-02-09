#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_EnumElm : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        //FIXME add explicit values
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_EnumElm : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong   getCertaintyThreshold() const override;
    };
}