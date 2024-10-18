#include "Module.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Module::getCategoryName(bool plural) const { return plural ? "module" : "modules"; }








void cmp::Pattern_Elm_Module::init() {
    __base_Pattern_Composite::__internal_init(
        op::Optional(op::Loop(op::OneOf(
            re::Alias(),
            re::Namespace(),
            re::Struct(),
            re::Enum()
        )))
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Module::generateData(std::vector<__base_ST*> const &results) const {
    ST_Module* r = new ST_Module;
    for(ulong i = 0; i < results.size(); ++i) {
        r->addChild(results[i]);
    }
    return dynamic_cast<__base_ST*>(r);
}


ulong cmp::Pattern_Elm_Module::getCertaintyThreshold() const {
    return (ulong)-1;
}