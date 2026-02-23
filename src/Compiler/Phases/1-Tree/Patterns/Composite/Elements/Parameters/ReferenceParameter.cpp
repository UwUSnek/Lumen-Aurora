#include "ReferenceParameter.hpp"
#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>

std::string cmp::ST_ReferenceParameter::getCategoryName(bool plural) const {
    return plural ? "reference parameters" : "reference parameter";
}

std::string cmp::Pattern_Elm_ReferenceParameter::genDecoratedValue(bool article) const {
    return article
        ? "a Reference Parameter"
        :   "Reference Parameter"
    ;
}

ulong cmp::Pattern_Elm_ReferenceParameter::getCertaintyThreshold() const {
    return 1;
}





void cmp::Pattern_Elm_ReferenceParameter::init() {
    using enum ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::Type(),
        tk::Identifier(),
        op::Optional(1UL,
            tk::Keyword(ReservedTokenId::META_KEYWORD_ASSIGN),
            tk::Identifier() //BUG implement expressions
        )
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Elm_ReferenceParameter::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_ReferenceParameter>();

    r->type = results[0]->asType();
    r->name = results[1]->asIdentifier();
    //BUG implement expressions & default values

    // Print debug info and return
    debug(console::cout << "Found reference parameter " << r->name << "\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}