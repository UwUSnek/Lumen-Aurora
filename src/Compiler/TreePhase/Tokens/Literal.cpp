#include "Literal.hpp"




std::string cmp::Pattern_Literal::genDecoratedValue() const {
    return "A Literal";
}




ulong cmp::Pattern_Literal::getCertaintyThreshold() const {
    return 1;
}