#pragma once
#include "Compiler/Phases/1-Tree/SourceTree.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_Function : public virtual __base_ST {
        ST_FunctionSignature *signature;
        std::vector<std::pair<std::optional<ptr<ST_Constraint>>, ptr<ST_RoutineBody>>> bodies;
        //! Identifier and return type are stored in the signature

        std::string getCategoryName(bool plural = false) const override;
    };




    struct Pattern_Elm_Function : public virtual __base_Pattern_Composite {
        void init();
        ptr<__base_ST> generateData(std::vector<ptr<__base_ST>> const &results) const override;
        std::string genDecoratedValue(bool article) const override;
        ulong getCertaintyThreshold() const override;
    };
}