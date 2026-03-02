#include "ANY_Statement.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::Pattern_ANY_Statement::genDecoratedValue(bool article) const {
    return article
        ? "a statement"
        :   "statement"
    ;
}

ulong cmp::Pattern_ANY_Statement::getCertaintyThreshold() const {
    return 1;
}








void cmp::Pattern_ANY_Statement::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            re::Sttm_If(),
            re::Sttm_March(),
            re::Sttm_For(),
            re::Sttm_While(),
            re::Sttm_DoWhile(),
            re::Sttm_Continue(),
            re::Sttm_Break(),
            re::Sttm_Once(),
            re::Sttm_Then(),
            re::Sttm_Return(),
            re::Sttm_Defer(),
            re::Sttm_Else(),
            re::Sttm_UnnamedScope(),
            re::Sttm_ExpressioStatement(),
            re::Sttm_EmptyStatement()
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_ANY_Statement::generateData(std::vector<ptr<__base_ST>> const &results) const {
    debug(console::cout << "found statement\n";)
    return std::dynamic_pointer_cast<__base_ST>(results[0]);
}