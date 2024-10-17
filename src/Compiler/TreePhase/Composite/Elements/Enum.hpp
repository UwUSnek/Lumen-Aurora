#pragma once
#include "Compiler/TokenizationPhase/TokenizedSource.hpp"
#include "Compiler/TreePhase/SourceTree.hpp"
#include "Compiler/TreePhase/Composite/__base_Pattern_Composite.hpp"








namespace cmp {
    struct ST_Enum : public virtual __base_ST_Referable, public virtual __base_ST_Container {
        ST_Sub_Path *baseType; //FIXME use type path
        //! Enum elements are saved in the children vector

        std::string getCategoryName(bool plural = false) const override;
    };




    //TODO set metakeyword string names to "meta keyword" (they are currently being referred to as "keywords")
    struct Pattern_Elm_Enum : public virtual __base_Pattern_Composite {
        void init();
        __base_ST* generateData(std::vector<__base_ST*> const &results) const override;
        std::string genDecoratedValue() const override;
        ulong   getCertaintyThreshold() const override;
    };
}