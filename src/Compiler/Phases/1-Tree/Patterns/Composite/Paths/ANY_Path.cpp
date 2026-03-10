#include "ANY_Path.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::Pattern_ANY_Path::genDecoratedValue(bool article) const {
    return article
        ? "a path"
        :   "path"
    ;
}

ulong cmp::Pattern_ANY_Path::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_ANY_Path::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            re::Path_Basic(),
            re::Path_TemporaryVariable(),
            re::Path_Template(),
            re::Path_Reflection(),
            re::ANY_Type() //TODO check if this causes an infinite loop.
            //TODO ANY_Type -> BasicType -> ANY_Path
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_ANY_Path::generateData(std::vector<ptr<__base_ST>> const &results) const {
    debug(console::cout << "found path\n";)
    return std::dynamic_pointer_cast<__base_ST>(results[0]);
}