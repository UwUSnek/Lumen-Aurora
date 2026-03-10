#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"
#include <cstddef>








namespace cmp {

    //! Template paths can decay into operator calls in subsequent phases.
    //! Only if used in expression contexts and the symbol doesn't match known templates and none of the arguments are valid type paths.

    struct ST_Path_Template : public virtual __base_ST {
        ptr<__base_ST> symbol;
        std::vector<ptr<__base_ST>> arguments;
        //! template parameters stored in children
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Path_Template : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}