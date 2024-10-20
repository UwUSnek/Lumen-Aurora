#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Type.hpp"







namespace cmp {
    struct ST_FunctionType : public virtual ST_Type {
        ST_Type              *retType  = nullptr;
        std::vector<ST_Type*> argTypes;
        ulong pointerNum = 0;

        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Type_Function : public virtual Pattern_Elm_Type {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong   getCertaintyThreshold() const override;
    };
}