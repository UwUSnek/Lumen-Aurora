#include "Module.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Module::getCategoryName(bool plural) const {
    return plural ? "module" : "modules";
}

std::string cmp::Pattern_Elm_Module::genDecoratedValue(bool article) const {
    return article
        ? "A module"
        : "Module"
    ;
}








void cmp::Pattern_Elm_Module::init() {
    __base_Pattern_Composite::__internal_init(
        op::Optional(0, op::Loop((ulong)-1, op::OneOf(
            re::Import(),
            re::Export(),
            re::Alias(),
            re::Namespace(),
            re::Struct(),
            re::Enum()
        ))
    ));
}




cmp::__base_ST* cmp::Pattern_Elm_Module::generateData(std::vector<__base_ST*> const &results) const {
    auto* r = new ST_Module;
    for(const auto &child : results) {
        r->addChild(child);
    }
    return dynamic_cast<__base_ST*>(r);
}


ulong cmp::Pattern_Elm_Module::getCertaintyThreshold() const {
    return (ulong)-1;
}