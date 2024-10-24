#include "__base_Pattern_Composite.hpp"
#include "Compiler/TreePhase/PatternGenerators.hpp"




void cmp::__base_Pattern_Composite::initParentReferences(){
    for(auto const &elm : v) {
        if(elm->isOperator()) {
            elm->asOperator()->parent = dynamic_cast<__base_Pattern*>(this);
        }
    }
}