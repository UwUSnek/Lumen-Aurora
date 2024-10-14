#include "treePhase.hpp"
#include "Utils/errors.hpp"
#include "Parsers/Generic/scope.hpp"



//FIXME add expressions




cmp::__base_ST* cmp::generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool fatal) {
    ulong i = index;


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




    // Parse OneOf operator
    if(pattern->isOneOf()) {
        __Pattern_Operator_OneOf* p = pattern->asOneOf();

        //TODO maybe optimize OneOf to check all patterns at once? instead of going back to the list if one fails
        for(ulong j = 0; j < p->v.size(); ++j) {
            __base_Pattern* pElm = dynamic_cast<__base_Pattern*>(p->v[j]);
            __base_ST* elm = generateTree(pElm, b, i, false);
            if(elm) return elm;
        }
        return nullptr;

        //FIXME use OneOf to force at least one element
        //FIXME add an Optional operator
    }




    // Parse composite patterns
    if(pattern->isComposite()) {
        __base_Pattern_Composite* p = pattern->asComposite();

        std::vector<__base_ST*> elms;
        for(ulong j = 0; j < p->v.size(); ++j) {
            __base_Pattern* pElm = dynamic_cast<__base_Pattern*>(p->v[j]);

            // Parse Loop operator
            if(pElm->isLoop()) {
                __Pattern_Operator_Loop* pLoop = pElm->asLoop();

                for(ulong k = 0;; ++k) {
                    __base_Pattern* pLoopElm = dynamic_cast<__base_Pattern*>(pLoop->v[k]);
                    __base_ST* elm = generateTree(pLoopElm, b, i, fatal); //FIXME determine when fatal should be used and when not.
                    if(!elm) break;


                    i += elm->tokenEnd - elm->tokenBgn + 1;
                    elms.push_back(elm);
                    if(k >= pLoop->v.size()) k = 0;
                }
                //TODO this should prob print an error if fatal is true
            }
            else {
                __base_ST* elm = generateTree(pElm, b, i, fatal); //FIXME determine when fatal should be used and when not.

                if(!elm) {
                    if(fatal) utils::printError(
                        ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                        ElmCoords(b, index, i),
                        ElmCoords(b, i,     i),
                        ", but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                        //FIXME ^ print expected token
                    );
                    else return nullptr;
                }
                else {
                    i += elm->tokenEnd - elm->tokenBgn + 1;
                }
                elms.push_back(elm);
                //FIXME move disallowed child check to somewhere else.
                //FIXME maybe add a "generic" check when one fails fatally
                // if(!p->isChildAllowed(elm)) {
                //     utils::printErrorGeneric(ERROR_CMD_IMPORT_PATH_IS_FILE, "Disallowed child"); //FIXME
                //     //FIXME error message, add "disallowed" elements to the pattern
                //     //TODO maybe add a class for the disallowed elements instead of checking with a function
                // }
            }
        }

        __base_ST* r = p->generateData(elms);
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return r;
    }



    // Parse keyword tokens
    if(pattern->isKeyword()) {
        Pattern_Keyword* p = pattern->asKeyword();
        std::optional<Token> const &t = (*b)[index];

        if(!t.has_value() || !t->isKeyword(p->id)) {
            return nullptr;
        }

        ++i;
        __base_ST* r = dynamic_cast<__base_ST*>(new ST_Sub_Keyword(t->getValue_Keyword()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return r;
    }




    // Parse identifier tokens
    if(pattern->isIdentifier()) {
        std::optional<Token> const &t = (*b)[index];

        if(!t.has_value() || !t->isIdentifier()) {
            return nullptr;
        }

        ++i;
        __base_ST* r = dynamic_cast<__base_ST*>(new ST_Sub_Identifier(t->getValue_Identifier()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return r;
    }




    // if(pattern->isLiteral()) {
    //     __base_Pattern_Token* p = pattern->asLitisLiteral();

    // }

    //! Bogus return value to silence GCC
    return (__base_ST*)0xDEAD;
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
    *r->cpp() = dynamic_cast<ST_Module*>(generateTree(re::Module(), b, 0, true));




    r->closePipe();
}
//TODO put const everywhere it's needed