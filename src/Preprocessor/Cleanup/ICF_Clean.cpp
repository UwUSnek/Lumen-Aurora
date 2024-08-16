#include "ICF_Clean.hpp"




namespace pre {
    std::string IFC_CleanTypeToString(IFC_CleanType e) {
        switch(e) {
            case IFC_CleanType::STRING: return "STRING";
            case IFC_CleanType::CHAR:   return "CHAR";
            case IFC_CleanType::OTHER:  return "OTHER";
        }
        return "NONE";
    }
}