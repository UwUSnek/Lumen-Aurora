#include "IntermediateCodeFormat.hpp"




namespace pre {
    std::string IFC_CleanTypeToString(IFC_ElmType e) {
        switch(e) {
            case IFC_ElmType::STRING: return "STRING";
            case IFC_ElmType::CHAR:   return "CHAR";
            case IFC_ElmType::MACRO:  return "MACRO";
            case IFC_ElmType::OTHER:  return "OTHER";
        }
        return "NONE";
    }
}