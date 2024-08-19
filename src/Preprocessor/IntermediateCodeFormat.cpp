#include "IntermediateCodeFormat.hpp"




namespace pre {
    std::string IFC_CleanTypeToString(ICF_ElmType e) {
        switch(e) {
            case ICF_ElmType::STRING: return "STRING";
            case ICF_ElmType::CHAR:   return "CHAR";
            case ICF_ElmType::MACRO:  return "MACRO";
            case ICF_ElmType::OTHER:  return "OTHER";
        }
        return "NONE";
    }
}