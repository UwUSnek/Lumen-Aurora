#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {

    //! Simple reference to a known type. This is valid for:
    //!     - Template type parameters
    //!     - Declared enums
    //!     - Declared structs
    //!     - Variables
    //!     - Function values
    struct ST_Type_Basic : public virtual __base_ST {
        ptr<__base_ST> path;
        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Type_Basic : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}