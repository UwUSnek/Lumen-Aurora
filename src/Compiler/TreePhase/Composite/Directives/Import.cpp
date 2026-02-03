#include "Import.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"

std::string cmp::ST_Import::getCategoryName(bool plural) const {
    return plural ? "import directive" : "importe directives";
}

std::string cmp::Pattern_Elm_Import::genDecoratedValue(bool article) const {
    return article
        ? "an Import directive"
        :    "Import directive"
    ;
}

ulong cmp::Pattern_Elm_Import::getCertaintyThreshold() const {
    return 1;
}









//FIXME fix failed optionals skipping the incomplete match
//FIXME fix failed optionals skipping the incomplete match
//FIXME fix failed optionals skipping the incomplete match
//FIXME fix failed optionals skipping the incomplete match
//FIXME fix failed optionals skipping the incomplete match
//FIXME fix failed optionals skipping the incomplete match
//FIXME fix failed optionals skipping the incomplete match


void cmp::Pattern_Elm_Import::init() {
    using enum cmp::ReservedTokenId;
    __base_Pattern_Composite::__internal_init(
        op::OneOf(
            op::Sequence(1,
                tk::Keyword(KEYWORD_FROM),
                tk::Identifier(), //FIXME use a string literal
                op::Optional(1,
                    tk::Keyword(META_KEYWORD_AS),
                    tk::Identifier()
                ),
                tk::Keyword(KEYWORD_IMPORT),
                re::Path(),
                op::Optional(1,
                    tk::Keyword(META_KEYWORD_AS),
                    tk::Identifier()
                ),
                op::Optional((ulong)-1, op::Loop(1,
                    tk::Keyword(KEYWORD_COMMA),
                    re::Path(),
                    op::Optional(1,
                        tk::Keyword(META_KEYWORD_AS),
                        tk::Identifier()
                    )
                ))
            ),
            op::Sequence(1,
                tk::Keyword(KEYWORD_IMPORT),
                tk::Identifier(), //FIXME use a string literal
                op::Optional(1,
                    tk::Keyword(META_KEYWORD_AS),
                    tk::Identifier()
                )
            )
        ),
        tk::Keyword(KEYWORD_SEMICOLON)
    );
}








cmp::__base_ST* cmp::Pattern_Elm_Import::generateData(std::vector<__base_ST*> const &results) const {
    auto *r = new ST_Import;
    ulong i;



    //  import  <name>  ;       //!
    //  import  <name>  as      a  ; //!
    //  from    <name>  import  a  ,       b   as  c   ;
    //  from    <name>  as      a  import  a,  b   as  c;




    // Save module path
    r->name = results[1]->asIdentifier()->s; //FIXME use a string literal, remove extension automatically, print error if not a valid identifier
    const ST_Sub_Keyword *w2;
    const ST_Sub_Keyword *w1 = results[2]->asKeyword();
    if(w1->id == ReservedTokenId::KEYWORD_SEMICOLON) goto ret;




    // Save alias if specified
    i = 2;
    if(w1->id == ReservedTokenId::META_KEYWORD_AS) {
        r->name = results[3]->asIdentifier()->s;
        i += 2; // Skip "as <name>"
    }
    w2 = results[4]->asKeyword();
    if(w2->id == ReservedTokenId::KEYWORD_SEMICOLON) goto ret;




    // Save elements if specified (2nd element is not ; (keyword "from" was used))
    ++i; // Skip "import"
    while(true) {

        // Save the path of the imported element
        __internal_ST_Import_Elm elm(results[i]->asPath());
        ++i;

        // If the next result element is a semicolon, save the element without renaming and stop parsing
        const ST_Sub_Keyword *kw = results[i]->asKeyword();
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




    // Print debug info and return value
    ret:
    debug((cout++ << "found import directive\n")--;)
    return dynamic_cast<__base_ST*>(r);
}


//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators
//FIXME print error if symbolic identifiers are used for anything other than operators