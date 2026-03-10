#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {

    //! Simple reference to a stored value. This is valid for:
    //!     - Routine parameters
    //!     - Variables
    //!     - Function values
    //!     - Temporary variables
    struct ST_Expr_ReferenceExpression : public virtual __base_ST {
        ptr<__base_ST> path;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Expr_ReferenceExpression : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}