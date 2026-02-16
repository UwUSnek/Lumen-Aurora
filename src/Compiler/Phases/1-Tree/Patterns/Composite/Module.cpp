#include "Module.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include <memory>

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
        op::Optional(0UL, op::Loop((ulong)-1, op::OneOf(
            re::Import(),
            re::Export(),
            re::Alias(),
            re::Namespace(),
            re::Struct(),
            re::Enum()
        ))
    ));
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_Module::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Module>();
    for(const auto &child : results) {
        r->addChild(child);
    }
    return std::dynamic_pointer_cast<__base_ST>(r);
}


ulong cmp::Pattern_Elm_Module::getCertaintyThreshold() const {
    return (ulong)-1;
}