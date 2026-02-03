#include "Export.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Export::getCategoryName(bool plural) const {
    return plural ? "export directive" : "exporte directives";
}

std::string cmp::Pattern_Elm_Export::genDecoratedValue(bool article) const {
    return article
        ? "an Export directive"
        :    "Export directive"
    ;
}

ulong cmp::Pattern_Elm_Export::getCertaintyThreshold() const {
    return 1;
}









void cmp::Pattern_Elm_Export::init() {
    __base_Pattern_Composite::__internal_init(
        tk::Keyword(ReservedTokenId::KEYWORD_EXPORT),
        op::Optional((ulong)-1,
            re::Path(),
            op::Optional(1,
                tk::Keyword(ReservedTokenId::META_KEYWORD_AS),
                tk::Identifier()
            ),
            op::Optional((ulong)-1, op::Loop(1,
                tk::Keyword(ReservedTokenId::KEYWORD_COMMA),
                re::Path(),
                op::Optional(1,
                    tk::Keyword(ReservedTokenId::META_KEYWORD_AS),
                    tk::Identifier()
                )
            ))
        ),
        tk::Keyword(ReservedTokenId::KEYWORD_SEMICOLON)
    );
}




cmp::__base_ST* cmp::Pattern_Elm_Export::generateData(std::vector<__base_ST*> const &results) const {
    auto *r = new ST_Export;

    // Save elements if specified (2nd element is not ;)
    if(!results[1]->isKeyword()) {
        for(ulong i = 1;;) {

            // Save the path of the exported element
            __internal_ST_Export_Elm elm(results[i]->asPath());
            ++i;

            // If the next result element is a semicolon, save the element without renaming and stop parsing
            ST_Sub_Keyword const *kw = results[i]->asKeyword();
            if(kw->id == ReservedTokenId::KEYWORD_SEMICOLON) {
                r->elms.push_back(elm);
                break;
            }

            // If it is a comma, save the current element without renaming it
            else if(kw->id == ReservedTokenId::KEYWORD_COMMA) {
                r->elms.push_back(elm);
                ++i; // Skip to next identifier
            }

            // If it is "as", rename the current element and save it
            else {
                elm.name = results[i + 1]->asIdentifier()->s;
                i += 2; // Skip to next separator or ;
                r->elms.push_back(elm);

                // Stop parsing if ; is found
                if(results[i]->asKeyword()->id == ReservedTokenId::KEYWORD_SEMICOLON) {
                    break;
                }
                ++i; // Skip to next identifier
            }
        }
    }

    debug((cout++ << "found export directive\n")--;)
    return dynamic_cast<__base_ST*>(r);
}