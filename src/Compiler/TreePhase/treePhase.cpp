#include "treePhase.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"
#include "Parsers/Generic/scope.hpp"



//FIXME add expressions

#include "Parsers/Elements/enum.hpp"


std::vector<cmp::__base_ST*> cmp::generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool fatal debug(, int indent)) {
    ulong i = index;
    debug((cout++ << __internal_repeat(ansi::bright_black + "│ " + ansi::reset, indent) << ansi::green << pattern << ansi::blue << " @" << i << " ")--;)


    // bool _0 = pattern->isComposite();
    // bool _1 = pattern->asComposite()->v[0]->isLoop();
    // bool _3 = pattern->asComposite()->v[0]->asLoop()->v[0]->isOneOf();
    // auto tmp = pattern->asComposite()->v[0]->asLoop()->v[0]->asOneOf();

    // bool _a = tmp->v[0]->isComposite();
    // auto tmp_a = tmp->v[0]->asComposite();
    // bool _a0 = tmp_a->v[0]->isKeyword();
    // bool _a1 = tmp_a->v[1]->isIdentifier();
    // bool _a2 = tmp_a->v[2]->isKeyword();
    // bool _a3 = tmp_a->v[3]->isLoop();
    // bool _a4 = tmp_a->v[4]->isKeyword();

    // bool _b = tmp->v[0]->isComposite();
    // auto tmp_b = tmp->v[1]->asComposite();
    // bool _b0 = tmp_b->v[0]->isKeyword();
    // bool _b1 = tmp_b->v[1]->isIdentifier();
    // bool _b2 = tmp_b->v[2]->isKeyword();
    // bool _b3 = tmp_b->v[3]->isComposite(); //path
    // bool _b4 = tmp_b->v[4]->isKeyword();
    // bool _b5 = tmp_b->v[5]->isLoop();
    // bool _b6 = tmp_b->v[6]->isKeyword();


    // auto _TEST_a0 = tmp_a->v[3]->asLoop()->v[0]->asOneOf()->v[0]->asComposite(); //Enum
    // auto _TEST_a1 = tmp_a->v[3]->asLoop()->v[0]->asOneOf()->v[1]->asComposite(); //Namespace
    // auto _TEST_b0 = tmp_b->v[5]->asLoop()->v[0]->asOneOf()->v[0]->asComposite(); //Enum
    // auto _TEST_b1 = tmp_b->v[5]->asLoop()->v[0]->asOneOf()->v[1]->asComposite(); //Namespace

    // auto _TEST_c = dynamic_cast<__base_Pattern_Composite*>(re::__internal_cache_Enum);


    // Parse OneOf operator
    if(pattern->isOneOf()) {
        debug((cout++ << ansi::bright_black << "One Of\n" << ansi::reset)--;)
        __Pattern_Operator_OneOf* p = pattern->asOneOf();

        //TODO maybe optimize OneOf to check all patterns at once? instead of going back to the list if one fails
        for(ulong j = 0; j < p->v.size(); ++j) {
            __base_Pattern* pElm = p->v[j];
            std::vector<__base_ST*> const &elms = generateTree(pElm, b, i, false debug(, indent + 1));
            if(!elms.empty()) return elms; //! Optional is not allowed as a direct child of OneOf
        }
        return {};
    }




    // Parse optional operator
    if(pattern->isOptional()) {
        debug((cout++ << ansi::bright_black << "Optional\n" << ansi::reset)--;)
        __Pattern_Operator_Optional* p = pattern->asOptional();
        std::vector<__base_ST*> r;

        // For each element of the optional's sequence
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            __base_Pattern* pElm = p->v[j];
            std::vector<__base_ST*> const &elms = generateTree(pElm, b, i, false debug(, indent + 1));

            // If the generation succeeds, save the result trees in r
            if(!elms.empty())
            for(ulong k = 0; k < elms.size(); ++k) {
                r.push_back(elms[k]);
                i += elms[k]->tokenEnd - elms[k]->tokenBgn + 1;
            }

            // If not, return failure
            else return {};
        }

        // Result sum of result trees
        return r;
    }




    // Parse Loop operator
    if(pattern->isLoop()) {
        debug((cout++ << ansi::bright_black << "Loop\n" << ansi::reset)--;)
        __Pattern_Operator_Loop* p = pattern->asLoop();
        std::vector<__base_ST*> r;

        // Repeat loop sequence until it fails
        for(ulong l = 0;; ++l) {
            ulong j;

            // For each element of the loop's sequence
            for(j = 0; j < p->v.size(); ++j) {

                // Try to generate its tree. Break if the generation fails
                __base_Pattern* pElm = p->v[j];
                std::vector<__base_ST*> const &elms = generateTree(pElm, b, i, fatal debug(, indent + 1));
                if(!pElm->isOptional() && elms.empty()) break;

                // If the generation succeeds, save all of the result trees and update i
                for(ulong k = 0; k < elms.size(); ++k) {
                    r.push_back(elms[k]);
                    i += elms[k]->tokenEnd - elms[k]->tokenBgn + 1;
                }
            }

            // Check iteration result
            if(j == 0) {                // If the iteration failed
                if(l > 0) return r;         // If at least 1 previous iteration succeeded, return the trees
                else return {};             // If this is the first, return failure
            }
        }

        //! Bogus return value to silence GCC
        return { (__base_ST*)0xDEAD };
    }




    // Parse composite patterns
    if(pattern->isComposite()) {
        debug((cout++ << ansi::bright_black << "Composite (" << pattern->genDecoratedValue() << ")\n" << ansi::reset)--;)
        __base_Pattern_Composite* p = pattern->asComposite();

        // For each of element of the composite's sequence
        std::vector<__base_ST*> genSource;
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            __base_Pattern* pElm = p->v[j];
            std::vector<__base_ST*> const &elms = generateTree(pElm, b, i, fatal debug(, indent + 1));

            // If the generation fails and fatal is true, print an error. If it's false, return failure
            if(!pElm->isOptional() && elms.empty()) {
                if(fatal) utils::printError(
                    ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                    ElmCoords(b, index, i),
                    ElmCoords(b, i,     i),
                    "Incomplete " + p->genDecoratedValue() + ".\n" +
                    pElm->genDecoratedValue() + " was expected, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                );
                else return {};
            }

            // If it succeeds, save the result trees in genSource and update i
            for(ulong k = 0; k < elms.size(); ++k) {
                i += elms[k]->tokenEnd - elms[k]->tokenBgn + 1;
                genSource.push_back(elms[k]);
            }

                //FIXME move disallowed child check to somewhere else.
                //FIXME maybe add a "generic" check when one fails fatally
                // if(!p->isChildAllowed(elm)) {
                //     utils::printErrorGeneric(ERROR_CMD_IMPORT_PATH_IS_FILE, "Disallowed child"); //FIXME
                //     //FIXME error message, add "disallowed" elements to the pattern
                //     //TODO maybe add a class for the disallowed elements instead of checking with a function
                // }
            // }
        }

        // Generate the tree of the composite element and set its beginning and end indices, then return it as the sole result
        __base_ST* r = p->generateData(genSource);
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return { r };
    }
    //FIXME fix trees' parent pointer not getting set



    // Parse keyword tokens
    if(pattern->isKeyword()) {
        debug((cout++ << ansi::bright_black << "Keyword\n" << ansi::reset)--;)
        Pattern_Keyword* p = pattern->asKeyword();
        std::optional<Token> const &t = (*b)[index];

        if(!t.has_value() || !t->isKeyword(p->id)) {
            return {};
        }

        ++i;
        __base_ST* r = dynamic_cast<__base_ST*>(new ST_Sub_Keyword(t->getValue_Keyword()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return { r };
    }




    // Parse identifier tokens
    if(pattern->isIdentifier()) {
        debug((cout++ << ansi::bright_black << "Identifier\n" << ansi::reset)--;)
        std::optional<Token> const &t = (*b)[index];

        if(!t.has_value() || !t->isIdentifier()) {
            return {};
        }

        ++i;
        __base_ST* r = dynamic_cast<__base_ST*>(new ST_Sub_Identifier(t->getValue_Identifier()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return { r };
    }




    // if(pattern->isLiteral()) {
    //     __base_Pattern_Token* p = pattern->asLitisLiteral();

    // }

    //! Bogus return value to silence GCC
    return { (__base_ST*)0xDEAD };
}



//FIXME constructing new pattern elements creates an infinite loop
//FIXME this can prob be fixed by building singletons automatically and redirecting subsequent construction calls to the existing instances




void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {
    // //FIXME parse
    // //FIXME print invalid places
    // std::vector<__base_ST*> const &elms = generic_parseScope(b, 0, false, nullptr);

    // r->lock();
    // r->cpp() = *parser.parse();
    // r->unlock();
    // //TODO

    //FIXME set *r

    // for(ulong i = 0; i < parser.v.size(); ++i) {

    // }
    // debug(\n\n)
    *r->cpp() = dynamic_cast<ST_Module*>(generateTree(re::Module(), b, 0, true debug(, 0))[0]);




    r->closePipe();
}
//TODO put const everywhere it's needed