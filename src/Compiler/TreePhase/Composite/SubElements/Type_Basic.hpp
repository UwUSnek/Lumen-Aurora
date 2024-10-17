#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Type.hpp"







namespace cmp {
    struct ST_BasicType : public virtual ST_Type {
        ST_Sub_Path *path = nullptr;
        bool    isPointer = false;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Type_Basic : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue() const override;
        ulong   getCertaintyThreshold() const override;
    };
}