#include "treePhase.hpp"
#include "Utils/errors.hpp"
#include "Parsers/Generic/scope.hpp"



//FIXME add expressions




cmp::__base_ST* cmp::generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool fatal) {
    ulong i = index;


    // Parse composite patterns
    if(pattern->isComposite()) {
        //FIXME Pattern_Operator_Loop
        //FIXME Pattern_Operator_OneOf
        __base_Pattern_Composite* p = pattern->asComposite();

        std::vector<__base_ST*> elms;
        for(int i = 0; i < p->v.size(); ++i) {
            if(p->v[i]->isOperatorLoop()) {
                //TODO do something about it
            }

            __base_ST* elm = generateTree(p->v[i], b, i, true);
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
            //FIXME move disallowed child check to somewhere else.
            //FIXME maybe add a "generic" check when one fails fatally
            // if(!p->isChildAllowed(elm)) {
            //     utils::printErrorGeneric(ERROR_CMD_IMPORT_PATH_IS_FILE, "Disallowed child"); //FIXME
            //     //FIXME error message, add "disallowed" elements to the pattern
            //     //TODO maybe add a class for the disallowed elements instead of checking with a function
            // }
            elms.push_back(elm);
            if(elm) {
                i += elm->tokenEnd - elm->tokenBgn + 1;
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

        __base_ST* r = new ST_Sub_Keyword(t->getValue_Keyword());
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

        __base_ST* r = new ST_Sub_Identifier(t->getValue_Identifier());
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        return r;
    }




    // if(pattern->isLiteral()) {
    //     __base_Pattern_Token* p = pattern->asLitisLiteral();

    // }


    //FIXME
    return nullptr;
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
    *r->cpp() = *dynamic_cast<ST_Module*>(generateTree(new Pattern_Elm_Module(), b, 0, true));




    r->closePipe();
}
//TODO put const everywhere it's needed