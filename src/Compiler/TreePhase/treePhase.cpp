#include "treePhase.hpp"
#include "Utils/errors.hpp"
#include "Utils/ansi.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"



//FIXME add expressions


debug(
    static std::string genIndentation(int indent) { return cmp::__internal_repeat(ansi::bright_black + "│ " + ansi::reset, indent);   }
    static void        printFail     (int indent) { (cout++ << genIndentation(indent) << ansi::bold_bright_red   << "[FAIL]\n" << ansi::reset)--; }
    static void        printSuccess  (int indent) { (cout++ << genIndentation(indent) << ansi::bold_bright_green << "[OK]  \n" << ansi::reset)--; }
)








cmp::GenerationResult *cmp::generateTree(__base_Pattern* pattern, TokenizedSource *b, ulong index, bool optional debug(, int indent)) {
    ulong i = index;
    debug((cout++ << genIndentation(indent) << ansi::green << pattern << ansi::bright_black << " @" << i << " ")--;)




    // Parse OneOf operator
    if(pattern->isOneOf()) {
        debug((cout++ << ansi::bright_black << "One Of\n" << ansi::reset)--;)
        __Pattern_Operator_OneOf* p = pattern->asOneOf();

        // For each element of the OneOf's sequence
        GenerationResult* max = nullptr;
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            __base_Pattern* pElm = p->v[j];
            GenerationResult *result = generateTree(pElm, b, i, true debug(, indent + 1));

            // If the generation succeeds, return the result trees
            //! Optional is not allowed as a direct child of OneOf. No need to check
            if(result->isComplete) {
                debug(printSuccess(indent);)
                return result;
            }

            // If the failed generation is the one with the most matches so far, save it in max
            else if(j == 0 || result->trees.size() > max->trees.size()) max = result;
        }

        // If no element generates successfully, return the longest incomplete element
        debug(printFail(indent);)
        return max;
    }




    // Parse optional operator
    if(pattern->isOptional()) {
        debug((cout++ << ansi::bright_black << "Optional\n" << ansi::reset)--;)
        __Pattern_Operator_Optional* p = pattern->asOptional();
        GenerationResult *r = new GenerationResult{{}, true };

        // For each element of the optional's sequence
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            __base_Pattern* pElm = p->v[j];
            GenerationResult *result = generateTree(pElm, b, i, true debug(, indent + 1));

            // Save the result trees in r
            for(ulong k = 0; k < result->trees.size(); ++k) {
                r->trees.push_back(result->trees[k]);
                i += result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
            }

            // If the generation failed, mark r as failed and return the elements that were matched so far
            if(!result->isComplete) {
                debug(printFail(indent);)
                r->isComplete = false;
                delete result;
                return r;
            }
            delete result;
        }

        // Return all the result trees
        debug(printSuccess(indent);)
        return r;
    }




    // Parse Loop operator
    if(pattern->isLoop()) {
        debug((cout++ << ansi::bright_black << "Loop\n" << ansi::reset)--;)
        __Pattern_Operator_Loop* p = pattern->asLoop();
        GenerationResult *r = new GenerationResult{{}, true };

        // Repeat loop sequence until it fails
        for(ulong l = 0;; ++l) {
            ulong j;

            // For each element of the loop's sequence
            for(j = 0; j < p->v.size(); ++j) {

                // Try to generate its tree
                __base_Pattern* pElm = p->v[j];
                GenerationResult *result = generateTree(pElm, b, i, optional debug(, indent + 1));


                //               |                                 |
                //    isComplete | 1 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0 |
                //          l==0 | 1 1 1 1 0 0 0 0 1 1 1 1 0 0 0 0 |
                //    isOptional | 1 1 0 0 1 1 0 0 1 1 0 0 1 1 0 0 |
                //      optional | 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 |
                //               |                                 |
                //    save elms? | 1 1 1 1 1 1 1 1 1 1 1 1 0 0 0 0 | isComplete || l==0
                // stop parsing? | 0 0 0 0 0 0 0 0 1 1 0 0 1 1 0 0 | !isComplete && !isOptional
                //               |                                 |


                // If the generation succeeds or this is the first iteration, save all of the result trees and update i
                if(result->isComplete || l == 0) {
                    for(ulong k = 0; k < result->trees.size(); ++k) {
                        r->trees.push_back(result->trees[k]);
                        i += result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                    }
                }

                // If it fails and the result is not optional, stop parsing
                if(!result->isComplete && !pElm->isOptional()) {
                    delete result;
                    break;
                }
                delete result;
            }


            // Check iteration result
            if(j < p->v.size()) {

                // Set generation result and return
                r->isComplete = l > 0;
                debug(if(!r->isComplete) printFail(indent);)
                return r;
            }
        }


        //! Bogus return value to silence GCC
        return new GenerationResult{{ (__base_ST*)0xDEAD }, false };
    }




    // Parse composite patterns
    if(pattern->isComposite()) {
        debug((cout++ << ansi::bold_bright_magenta << "Composite (" << pattern->genDecoratedValue(false) << ")\n" << ansi::reset)--;)
        __base_Pattern_Composite* p = pattern->asComposite();

        // For each of element of the composite's sequence
        std::vector<__base_ST*> genSource;
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            __base_Pattern* pElm = p->v[j];
            GenerationResult *result = generateTree(pElm, b, i, optional debug(, indent + 1));

            // Save the result trees in genSource and update i
            for(ulong k = 0; k < result->trees.size(); ++k) {
                i += result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                genSource.push_back(result->trees[k]);
            }




            // Print an error if the generation fails
            // (And the certainty threshold of the current composite pattern has been reached)  //! Reaching the threshold means no other pattern is possible //FIXME idk if this is ok
            // (And there is at least 1 mandatory sub-pattern after the one that failed)        //! If the remaining ones are all optional, the parsing is a success
            // (And the current composite pattern is not part of an optional pattern)           //! Children of optionals fail silently
            if(!result->isComplete) {


                // Check if the current pattern and all the remaining ones are optional
                bool allOptional = true;
                for(ulong k = j; k < p->v.size(); ++k) {
                    if(!p->v[k]->isOptional()) {
                        allOptional = false;
                        break;
                    }
                }


                if(j >= p->getCertaintyThreshold()) { //FIXME check if optionals need to be counted. they might be messing up the threshold detection
                    // if(!allOptional && !optional) {
                    if(!allOptional) {

                        // Find the element that caused the error (skip operators)
                        std::string expectedElementStr;
                        // for(__base_Pattern* curPattern = pElm; curPattern = curPattern->asOperator()->v[0];) { //FIXME check if it's always [0] or it can be other indices as well
                        //FIXME check if it's always [0] or it can be other indices as well
                        for(__base_Pattern* curPattern = pElm;; curPattern = curPattern->asOperator()->v[0]) {
                            // if(curPattern->isComposite() || curPattern->isToken()) {
                            if(!curPattern->isOperator()) {
                                expectedElementStr = curPattern->genDecoratedValue(true);
                                break;
                            }
                        }
                        expectedElementStr[0] = std::toupper(expectedElementStr[0]);


                        // Actually print the error
                        utils::printError(
                            ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                            ElmCoords(b, index, i),
                            ElmCoords(b, i,     i),
                            "Incomplete " + p->genDecoratedValue(false) + ".\n" +
                            expectedElementStr + " was expected, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                            //FIXME pElm can be an operator - do something about it
                        );
                        //FIXME list possible elements when none of a OneOf's choices are found, instead of saying "expected <firstElement>, but..."
                    }
                }
                else {
                    debug(printFail(indent);)
                    delete result;
                    return new GenerationResult{{}, false };
                }
            }
            delete result;
        }

        // Generate the tree of the composite element and set its beginning and end indices, then return it as the sole result
        __base_ST* r = p->generateData(genSource);
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        debug(printSuccess(indent);)
        return new GenerationResult{{ r }, true };
    }
    //FIXME fix trees' parent pointer not getting set




    // Parse keyword tokens
    if(pattern->isKeyword()) {
        debug((cout++ << ansi::blue << "Keyword\n" << ansi::reset)--;)
        Pattern_Keyword* p = pattern->asKeyword();
        std::optional<Token> const &t = (*b)[index];

        if(!t.has_value() || !t->isKeyword(p->id)) {
            debug(printFail(indent);)
            return new GenerationResult{{}, false};
        }

        ++i;
        __base_ST* r = dynamic_cast<__base_ST*>(new ST_Sub_Keyword(t->getValue_Keyword()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        debug(printSuccess(indent);)
        return new GenerationResult{{ r }, true };
    }




    // Parse identifier tokens
    if(pattern->isIdentifier()) {
        debug((cout++ << ansi::blue << "Identifier\n" << ansi::reset)--;)
        std::optional<Token> const &t = (*b)[index];

        if(!t.has_value() || !t->isIdentifier()) {
            debug(printFail(indent);)
            return new GenerationResult{{}, false};
        }

        ++i;
        __base_ST* r = dynamic_cast<__base_ST*>(new ST_Sub_Identifier(t->getValue_Identifier()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        debug(printSuccess(indent);)
        return new GenerationResult{{ r }, true };
    }




    // if(pattern->isLiteral()) {
    //     __base_Pattern_Token* p = pattern->asLitisLiteral();

    // }




    //! Bogus return value to silence GCC
    return new GenerationResult{{ (__base_ST*)0xDEAD }, false };
}








void cmp::startTreePhase(TokenizedSource *b, SourceTree *r) {
    *r->cpp() = dynamic_cast<ST_Module*>(generateTree(re::Module(), b, 0, false debug(, 0))->trees[0]);
    r->closePipe();
}
//TODO put const everywhere it's needed
//FIXME free (delete) all the unnecessary GenerationResult s