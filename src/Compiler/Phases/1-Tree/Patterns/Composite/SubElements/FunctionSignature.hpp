#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_FunctionSignature : public virtual __base_ST_Referable, public virtual __base_ST_Typed {
        //BUG rename "routine parameters" to "simple parameters". routine parameters should encompass all types of parameters. use a __base_ST_Parameter for that
        // std::vector<ptr<__base_ST_Parameter>> parameters; //BUG implement parameters
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_FunctionSignature : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong   getCertaintyThreshold() const override;
    };
}