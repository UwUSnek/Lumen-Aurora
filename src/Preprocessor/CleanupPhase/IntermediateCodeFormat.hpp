//!                                                           ICF                                                           !//
//!                                                 Intermediate Code Format                                                !//


#pragma once
#include <string>
#include <vector>
#include <regex>
#include "Utils/utils.hpp"




namespace pre {
    enum class IFC_ElmType {
        NONE,
        STRING,
        CHAR,
        MACRO,
        OTHER
    };
    std::string IFC_CleanTypeToString(IFC_ElmType e);




    /**
     * @brief Represents a chunk of code created by the cleanup phase.
     *      t:          The type of the value.
     *      s:          The clean string value, or the name of the macro in case of macros. //TODO create map with name as key and macro structure as value
     *      OG_s:       The raw string value.
     *      OG_lineNum: The line number of the raw value in the original file.
     *      OG_height:  The number of NLCs the raw value contained.
     *      OG_start:   The index of the start of the raw value in the original file (first character).
     */
    class ICF_Elm {
    public:
        IFC_ElmType t;
        std::string s;
        std::string OG_s;
        ulong OG_lineNum;  //! Redundant, used for performance reasons
        ulong OG_height;   //! Redundant, used for performance reasons
        ulong OG_start;


        ICF_Elm(IFC_ElmType _t, std::string _s, std::string _OG_s, ulong _OG_lineNum, ulong _OG_height, ulong _OG_start) :
            t(_t),
            s(_s),
            OG_s(_OG_s),
            OG_lineNum(_OG_lineNum),
            OG_height(_OG_height),
            OG_start(_OG_start) {
        }
    };




    /**
     * @brief Represents the source code after the cleanup phase.
     *      Each element contains an contiguous chunk of code (meaning that it didn't contain any whitespace in the original file).
     *      LCTs do not split the code.
     */
    class IntermediateCodeFormat {
    public:
        std::vector<ICF_Elm> elms;

        constexpr ICF_Elm& operator[](std::size_t i) noexcept {
            return elms[i];
        }


        std::string toString(){
            static ulong elmTypeNameMaxSpace = 16;
            static ulong lineValueMaxSpace = 64;
            std::string r = "";
            for(ICF_Elm &elm : elms) {

                // Type
                std::string elmTypeName = IFC_CleanTypeToString(elm.t);
                r += elmTypeName;
                r.append(std::max(1l, (long)elmTypeNameMaxSpace - (long)elmTypeName.length()), ' ');


                // Clean value
                r += "| ";
                std::string elmValue = std::regex_replace(elm.s, std::regex(" "), ansi::black + "·" + ansi::reset);
                r += elmValue;
                r.append(std::max(1l, (long)lineValueMaxSpace - (long)elmValue.length()), ' ');


                // Raw value
                r += "| ";
                r += std::regex_replace(std::regex_replace(elm.OG_s, std::regex("\n"), ansi::black + "\\n" + ansi::reset), std::regex(" "), ansi::black + "·" + ansi::reset);
                r += "\n";
            }
            return r;
        }
    };
}