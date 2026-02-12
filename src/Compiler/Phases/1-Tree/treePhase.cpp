#include "treePhase.hpp"
#include "Main/ALC.hpp"
#include "Main/FatalErrorException.hpp"
#include "Main/errors.hpp"
#include "Compiler/Phases/1-Tree/PatternGenerators.hpp"
#include <memory>



//FIXME add expressions


debug(
    static std::string genIndentation(int indent) { return cmp::__internal_repeat(ansi::bright_black + "│ " + ansi::reset, indent);   }
    static void        printFail     (int indent) { cout << genIndentation(indent) << ansi::bold_bright_red   << "[FAIL]\n" << ansi::reset; }
    static void        printSuccess  (int indent) { cout << genIndentation(indent) << ansi::bold_bright_green << "[OK]  \n" << ansi::reset; }
)








ptr<cmp::TreeGenerationResult> cmp::generateTree(__base_Pattern *pattern, ptr<TokenizedSource<false>> b, ulong index, bool optional debug(, int indent)) {
    ulong i = index;
    debug(cout << genIndentation(indent) << ansi::green << pattern << ansi::bright_black << " @" << i << " ";)




    // Parse OneOf operator
    if(pattern->isOneOf()) {
        debug(cout << ansi::bright_black << "One Of\n" << ansi::reset;)
        auto p = pattern->asOneOf();

        // For each element of the OneOf's sequence
        ptr<TreeGenerationResult> max = nullptr;
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            auto pElm = p->v[j];
            auto result = generateTree(pElm, b, i, true debug(, indent + 1));

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
    //FIXME account for custom threshold
    //FIXME FIX ALL OF 0-THRESHOLD FOR THIS OPERATOR
    if(pattern->isOptional()) {
        debug(cout << ansi::bright_black << "Optional\n" << ansi::reset;)
        __Pattern_Operator_Optional* p = pattern->asOptional();
        auto r = newptr<TreeGenerationResult>(0, true );

        // For each element of the optional's sequence
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            auto pElm = p->v[j];
            auto result = generateTree(pElm, b, i, true debug(, indent + 1));

            // Save the result trees in r
            for(ulong k = 0; k < result->trees.size(); ++k) {
                r->trees.push_back(result->trees[k]);
                i += result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                // const auto progress = result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                // increaseLocalProgress(progress);
                // i += progress;
            }

            // // If the generation failed, mark r as failed and return the elements that were matched so far
            // if(!result->isComplete) {
            // If the generation failed and the element is not optional, mark r as failed and return the elements that were matched so far
            // if(!result->isComplete && !pElm->isOptional()) {
            // if(!result->isComplete && (!pElm->isOptional() || i >= pElm->getCertaintyThreshold())) {
            if(!result->isComplete) {
                // if(result->trees.size() >= pElm->getCertaintyThreshold()) { //BUG prob needs to check if it's optional as well. or something like that
                if(j >= p->getCertaintyThreshold()) {
                    // Find the element that caused the error (walk up the tree and skip operators)
                    std::string parentElementStr;
                    for(__base_Pattern* curPattern = p;; curPattern = curPattern->asOperator()->parent) {
                        // if(curPattern->isComposite() || curPattern->isToken()) {
                        if(!curPattern->isOperator()) {
                            parentElementStr = curPattern->genDecoratedValue(false);
                            break;
                        }
                    }
                    parentElementStr[0] = (char)std::toupper(parentElementStr[0]);


                    // Find the element that caused the error (skip operators)
                    std::string expectedElementStr;
                    // for(__base_Pattern* curPattern = pElm; curPattern = curPattern->asOperator()->v[0];) { //FIXME check if it's always [0] or it can be other indices as well
                    //FIXME check if it's always [0] or it can be other indices as well
                    for(auto curPattern = pElm;; curPattern = curPattern->asOperator()->v[0]) {
                        // if(curPattern->isComposite() || curPattern->isToken()) {
                        if(!curPattern->isOperator()) {
                            expectedElementStr = curPattern->genDecoratedValue(true);
                            break;
                        }
                    }
                    expectedElementStr[0] = (char)std::toupper(expectedElementStr[0]);


                    // Actually print the error
                    utils::printError(
                        ErrorCode::ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                        ElmCoords(b, index, i), //FIXME bad starting coords for the relevant section
                        ElmCoords(b, i,     i), //FIXME bad starting coords for the relevant section
                        "Incomplete " + parentElementStr +
                        ((*b)[i]
                            ? ".\n" + expectedElementStr + " was expected, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                            : ": Unexpected end of file.\n" + expectedElementStr + " was expected."
                        ),
                        true //TODO recovery system. skip to the first token that makes sense
                    );
                }
                else {
                    // debug(printFail(indent);)
                    debug(printSuccess(indent);)
                    // r->isComplete = false;
                    // return r;
                    return newptr<TreeGenerationResult>(0, true );
                }
            }
        }

        // Return all the result trees
        debug(printSuccess(indent);)
        return r;
    }




    // Parse Sequence operator
    //FIXME account for custom threshold
    //FIXME FIX ALL OF 0-THRESHOLD FOR THIS OPERATOR
    if(pattern->isSequence()) {
        debug(cout << ansi::bright_black << "Sequence\n" << ansi::reset;)
        __Pattern_Operator_Sequence* p = pattern->asSequence();
        auto r = newptr<TreeGenerationResult>(0, true );

        // For each element of the sequence's sequence
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            auto pElm = p->v[j];
            auto result = generateTree(pElm, b, i, true debug(, indent + 1));

            // Save the result trees in r
            for(ulong k = 0; k < result->trees.size(); ++k) {
                r->trees.push_back(result->trees[k]);
                i += result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                // const auto progress = result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                // increaseLocalProgress(progress);
                // i += progress;
            }

            // If the generation failed and the element is not optional, mark r as failed and return the elements that were matched so far
            // if(!result->isComplete && !pElm->isOptional()) {
            // if(!result->isComplete && (!pElm->isOptional() || i >= pElm->getCertaintyThreshold())) {
            if(!result->isComplete) {
                if(j >= p->getCertaintyThreshold()) { //FIXME messed up error coords
                    // Find the element that caused the error (walk up the tree and skip operators)
                    std::string parentElementStr;
                    for(__base_Pattern* curPattern = p;; curPattern = curPattern->asOperator()->parent) {
                        // if(curPattern->isComposite() || curPattern->isToken()) {
                        if(!curPattern->isOperator()) {
                            parentElementStr = curPattern->genDecoratedValue(false);
                            break;
                        }
                    }
                    parentElementStr[0] = (char)std::toupper(parentElementStr[0]);


                    // Find the element that caused the error (skip operators)
                    std::string expectedElementStr;
                    // for(__base_Pattern* curPattern = pElm; curPattern = curPattern->asOperator()->v[0];) { //FIXME check if it's always [0] or it can be other indices as well
                    //FIXME check if it's always [0] or it can be other indices as well
                    for(auto curPattern = pElm;; curPattern = curPattern->asOperator()->v[0]) {
                        // if(curPattern->isComposite() || curPattern->isToken()) {
                        if(!curPattern->isOperator()) {
                            expectedElementStr = curPattern->genDecoratedValue(true);
                            break;
                        }
                    }
                    expectedElementStr[0] = (char)std::toupper(expectedElementStr[0]);


                    // Actually print the error
                    utils::printError(
                        ErrorCode::ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                        ElmCoords(b, index, i), //FIXME bad starting coords for the relevant section
                        ElmCoords(b, i,     i), //FIXME bad starting coords for the relevant section
                        "Incomplete " + parentElementStr +
                        ((*b)[i]
                            ? ".\n" + expectedElementStr + " was expected, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                            : ": Unexpected end of file.\n" + expectedElementStr + " was expected."
                        ),
                        true //TODO recovery system. skip to the first token that makes sense
                    );
                }
                else {
                    debug(printFail(indent);)
                    r->isComplete = false;
                    return r;
                }
            }
        }

        // Return all the result trees
        debug(printSuccess(indent);)
        return r;
    }




    // Parse Loop operator
    if(pattern->isLoop()) {
        debug(cout << ansi::bright_black << "Loop\n" << ansi::reset;)
        __Pattern_Operator_Loop* p = pattern->asLoop();
        auto r = newptr<TreeGenerationResult>(0, true );

        // Repeat loop sequence until it fails
        for(ulong l = 0;; ++l) {
            ulong j;

            // For each element of the loop's sequence
            for(j = 0; j < p->v.size(); ++j) {

                // Try to generate its tree
                auto pElm = p->v[j];
                auto result = generateTree(pElm, b, i, optional debug(, indent + 1));


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
                        // const auto progress = result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                        // increaseLocalProgress(progress);
                        // i += progress;
                    }
                }

                // If it fails and the result is not optional, stop parsing
                // if(!result->isComplete && !pElm->isOptional()) {
                // if(!result->isComplete && (!pElm->isOptional() || i >= pElm->getCertaintyThreshold())) {
                if(!result->isComplete) {
                    if(j >= p->getCertaintyThreshold()) { //FIXME messed up error coords
                        // Find the element that caused the error (walk up the tree and skip operators)
                        std::string parentElementStr;
                        for(__base_Pattern* curPattern = p;; curPattern = curPattern->asOperator()->parent) {
                            // if(curPattern->isComposite() || curPattern->isToken()) {
                            if(!curPattern->isOperator()) {
                                parentElementStr = curPattern->genDecoratedValue(false);
                                break;
                            }
                        }
                        parentElementStr[0] = (char)std::toupper(parentElementStr[0]);


                        // Find the element that caused the error (skip operators)
                        std::string expectedElementStr;
                        // for(__base_Pattern* curPattern = pElm; curPattern = curPattern->asOperator()->v[0];) { //FIXME check if it's always [0] or it can be other indices as well
                        //FIXME check if it's always [0] or it can be other indices as well
                        for(auto curPattern = pElm;; curPattern = curPattern->asOperator()->v[0]) {
                            // if(curPattern->isComposite() || curPattern->isToken()) {
                            if(!curPattern->isOperator()) {
                                expectedElementStr = curPattern->genDecoratedValue(true);
                                break;
                            }
                        }
                        expectedElementStr[0] = (char)std::toupper(expectedElementStr[0]);


                        // Actually print the error
                        utils::printError(
                            ErrorCode::ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                            ElmCoords(b, index, i), //FIXME bad starting coords for the relevant section
                            ElmCoords(b, i,     i), //FIXME bad starting coords for the relevant section
                            "Incomplete " + parentElementStr +
                            ((*b)[i]
                                ? ".\n" + expectedElementStr + " was expected, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                                : ": Unexpected end of file.\n" + expectedElementStr + " was expected."
                            ),
                            true //TODO recovery system. skip to the first token that makes sense
                        );
                        //FIXME list possible elements when none of a OneOf's choices are found, instead of saying "expected <firstElement>, but..."

                        //FIXME "Incomplete Module. Found unexpected Identifier "idk" //TODO this is already printed
                        //FIXME "Only one of these is allowed: Struct definition, Import directive, Export directive, Routine definition"
                    }
                    else {
                        break;
                    }
                }
            }


            // Check iteration result
            if(j < p->v.size()) {

                // Set generation result and return
                r->isComplete = l > 0;
                debug((r->isComplete ? printSuccess : printFail)(indent);)
                return r;
            }
        }


        //! Bogus return value to silence GCC
        return newptr<TreeGenerationResult>(0, false );
    }




    // Parse composite patterns
    if(pattern->isComposite()) {
        debug(cout << ansi::bold_bright_magenta << "Composite (" << pattern->genDecoratedValue(false) << ")\n" << ansi::reset;)
        auto* p = pattern->asComposite();

        // For each of element of the composite's sequence
        std::vector<ptr<__base_ST>> genSource;
        for(ulong j = 0; j < p->v.size(); ++j) {

            // Try to generate its tree
            auto pElm = p->v[j];
            auto result = generateTree(pElm, b, i, optional debug(, indent + 1));

            // Save the result trees in genSource and update i
            for(ulong k = 0; k < result->trees.size(); ++k) {
                genSource.push_back(result->trees[k]);
                i += result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                // const auto progress = result->trees[k]->tokenEnd - result->trees[k]->tokenBgn + 1;
                // increaseLocalProgress(progress);
                // i += progress;
            }




            // Print an error if the generation fails
            // (And the certainty threshold of the current composite pattern has been reached)  //! Reaching the threshold means no other pattern is possible //FIXME idk if this is ok
            // (And there is at least 1 mandatory sub-pattern after the one that failed)        //! If the remaining ones are all optional, the parsing is a success
            // (And the current composite pattern is not part of an optional pattern)           //! Children of optionals fail silently
            if(!result->isComplete) {


                if(j >= p->getCertaintyThreshold()) { //FIXME check if optionals need to be counted. they might be messing up the threshold detection

                    // // Check if the current pattern and all the remaining ones are optional
                    // Check if the remaining patterns are optional
                    // bool allOptional = true;
                    // for(ulong k = j + 1; k < p->v.size(); ++k) {
                    //     if(!p->v[k]->isOptional()) {
                    //         allOptional = false;
                    //         break;
                    //     }
                    // }

                    // if(!allOptional && !optional) {
                    // if(!allOptional && !pElm->isOptional()) {
                    // if(!pElm->isOptional()) {

                        // Find the element that caused the error (skip operators)
                        std::string expectedElementStr;
                        // for(__base_Pattern* curPattern = pElm; curPattern = curPattern->asOperator()->v[0];) { //FIXME check if it's always [0] or it can be other indices as well
                        //FIXME check if it's always [0] or it can be other indices as well
                        for(auto curPattern = pElm;; curPattern = curPattern->asOperator()->v[0]) {
                            // if(curPattern->isComposite() || curPattern->isToken()) {
                            if(!curPattern->isOperator()) {
                                expectedElementStr = curPattern->genDecoratedValue(true);
                                break;
                            }
                        }
                        expectedElementStr[0] = (char)std::toupper(expectedElementStr[0]);


                        // Actually print the error
                        utils::printError(
                            ErrorCode::ERROR_CMP_UNEXPECTED_TOKEN, utils::ErrType::COMPILER,
                            ElmCoords(b, index, i),
                            ElmCoords(b, i,     i),
                            "Incomplete " + p->genDecoratedValue(false) +
                            ((*b)[i]
                                ? ".\n" + expectedElementStr + " was expected, but the " + (*b)[i]->genDecoratedValue() + " was found instead."
                                : ": Unexpected end of file.\n" + expectedElementStr + " was expected."
                            ),
                            true //TODO recovery system. skip to the first token that makes sense
                        );
                        //FIXME list possible elements when none of a OneOf's choices are found, instead of saying "expected <firstElement>, but..."

                        //FIXME "Incomplete Module. Found unexpected Identifier "idk" //TODO this is already printed
                        //FIXME "Only one of these is allowed: Struct definition, Import directive, Export directive, Routine definition"
                    // }
                }
                else {
                    debug(printFail(indent);)
                    return newptr<TreeGenerationResult>(0, false );
                }
            }
        }

        // Generate the tree of the composite element and set its beginning and end indices, then return it as the sole result
        auto r = p->generateData(genSource);
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        debug(printSuccess(indent);)
        return newptr<TreeGenerationResult>(std::vector<decltype(r)>{ r }, true );
    }
    //FIXME fix trees' parent pointer not getting set




    // Parse keyword tokens
    if(pattern->isKeyword()) {
        debug(
            std::string keywordId;
            for(auto const &pair : reservedTokensMap) if(pair.second == pattern->asKeyword()->id) { keywordId = pair.first; break; }
            cout << ansi::blue << "Keyword " << keywordId << "\n" << ansi::reset;
        )
        const auto* p = pattern->asKeyword();
        const auto t = (*b)[index];

        if(!t || !t->isKeyword(p->id)) {
            debug(printFail(indent);)
            return newptr<TreeGenerationResult>(0, false);
        }

        increaseLocalProgress(1);
        ++i;
        auto r = std::dynamic_pointer_cast<__base_ST>(newptr<ST_Sub_Keyword>(t->getValue_Keyword()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        debug(printSuccess(indent);)
        return newptr<TreeGenerationResult>(std::vector<decltype(r)>{ r }, true );
    }




    // Parse identifier tokens
    if(pattern->isIdentifier()) {
        debug(cout << ansi::blue << "Identifier\n" << ansi::reset;)
        const auto t = (*b)[index];

        if(!t || !t->isIdentifier()) {
            debug(printFail(indent);)
            return newptr<TreeGenerationResult>(0, false);
        }

        increaseLocalProgress(1);
        ++i;
        auto r = std::dynamic_pointer_cast<__base_ST>(newptr<ST_Sub_Identifier>(t->getValue_Identifier()));
        r->tokenBgn = index;
        r->tokenEnd = i - 1;
        debug(printSuccess(indent);)
        return newptr<TreeGenerationResult>(std::vector<decltype(r)>{ r }, true );
    }




    //TODO
    // if(pattern->isLiteral()) {
    //     __base_Pattern_Token* p = pattern->asLitisLiteral();

    // }




    //! Bogus return value to silence GCC
    return newptr<TreeGenerationResult>(0, false );
}








void cmp::__internal_startTreePhase(ptr<TokenizedSource<false>> b, ptr<SourceTree> r) {
    const auto moduleTree = generateTree(re::Module(), b, 0, false debug(, 0));
    *r->cpp() = std::dynamic_pointer_cast<ST_Module>(moduleTree->trees[0]);
}








void cmp::startTreePhase(ptr<TokenizedSource<false>> b, ptr<SourceTree> r) {

    // Try to execute the subphase
    try {
        __internal_startTreePhase(b, r);
        r->closePipe();
    }

    // If errors occur, close the return pipe and return safely
    // This lets any dependant subphase join and the main thread exit the program
    catch(const FatalErrorException&) {
        r->closePipe();
        // std::scoped_lock lock(phaseDataArrayLock);
        // phaseDataArray[Compiler_TreeCreation].totalProgress->setProgressColor(ansi::red); //FIXME change progress bar color on failure
    }
}