#include "Compiler/Phases/0-Tokenization/TokenizedSource.hpp"
#include "TemplatePath.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include "Utils/console.hpp"
#include <memory>


std::string cmp::ST_Path_Template::getCategoryName(bool plural) const {
    return plural ? "template paths" : "template path";
}

std::string cmp::Pattern_Path_Template::genDecoratedValue(bool article) const {
    return article
        ? "a Template path"
        :   "Template path"
    ;
}

ulong cmp::Pattern_Path_Template::getCertaintyThreshold() const {
    return 2;
}








void cmp::Pattern_Path_Template::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        re::ANY_Path(),
        tk::Keyword(META_KEYWORD_LT),
        op::Optional(1UL, op::OneOf(
            re::ANY_Type(),
            re::ANY_Expression()
        )),
        op::Optional(1UL, op::Loop((ulong)-1,
            tk::Keyword(KEYWORD_COMMA),
            op::OneOf(
                re::ANY_Type(),
                re::ANY_Expression(),
            )
        )),
        tk::Keyword(META_KEYWORD_GT)
    );
}




ptr<cmp::__base_ST> cmp::Pattern_Path_Template::generateData(std::vector<ptr<__base_ST>> const &results) const {
    auto r = newptr<ST_Path_Template>();

    // Save value
    r->symbol = results[0];
    for(ulong i = 2; i < results.size() - 1; i += 2) {
        r->arguments.emplace_back(results[i]);
    }


    // Print debug info and return
    debug(console::cout << "found template path\n";)
    return std::dynamic_pointer_cast<__base_ST>(r);
}