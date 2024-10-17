#pragma once
#include "Compiler/TreePhase/__base_Pattern.hpp"
#include "ALC.hpp"




namespace cmp {
    debug(
        static std::string __internal_repeat(std::string s, ulong n) {
            std::stringstream r;
            for(ulong i = 0; i < n; ++i) r << s;
            return r.str();
        }
    )
    debug(void printPatternElmInfo(__base_Pattern* p, int indent);)
    struct __base_Pattern_Composite : public virtual __base_Pattern {
        std::vector<__base_Pattern*> v;
        template<class ...t> void __internal_init(t... _v) {
            v = std::vector<__base_Pattern*>{ dynamic_cast<__base_Pattern*>(_v)... };
            debug(
                cout++;
                cout << "\n" << this << " Composite pattern initialized with:\n";
                for(ulong i = 0; i < v.size(); ++i) {
                    printPatternElmInfo(v[i], 1);
                }
                cout << "\n";
                cout--;
            );
        }

        // virtual bool isChildAllowed(__base_ST* const child) const = 0;
        virtual __base_ST* generateData(std::vector<__base_ST*> const &parsedElements) const = 0;
    };
}