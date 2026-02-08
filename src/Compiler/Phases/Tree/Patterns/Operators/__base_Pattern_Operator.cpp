#include "__base_Pattern_Operator.hpp"




void cmp::__base_Pattern_Operator::initParentReferences(){
    for(auto const &elm : v) {
        if(elm->isOperator()) {
            elm->asOperator()->parent = dynamic_cast<__base_Pattern*>(this);
        }
    }
}