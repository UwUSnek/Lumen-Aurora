#include "__base_Pattern_Composite.hpp"
#include "Compiler/Phases/1-Tree/Patterns/Operators/__base_Pattern_Operator.hpp"




void cmp::__base_Pattern_Composite::initParentReferences(){
    for(auto const &elm : v) {
        if(elm->isOperator()) {
            elm->asOperator()->parent = dynamic_cast<__base_Pattern*>(this);
        }
    }
}