#include "ANY_Type.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::Pattern_ANY_Type::genDecoratedValue(bool article) const {
    return article
        ? "a type"
        :   "type"
    ;
}

ulong cmp::Pattern_ANY_Type::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_ANY_Type::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            re::Type_Typeof(),
            re::Type_Auto(),
            re::Type_Function(),
            re::Type_Basic()
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_ANY_Type::generateData(std::vector<ptr<__base_ST>> const &results) const {
    debug(console::cout << "found type\n";)
    return std::dynamic_pointer_cast<__base_ST>(results[0]);
}