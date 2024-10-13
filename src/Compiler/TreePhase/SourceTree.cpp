#include "SourceTree.hpp"
#include "Parsers/Elements/alias.hpp"
#include "Parsers/Elements/enum.hpp"
#include "Parsers/Elements/namespace.hpp"
#include "Parsers/SubElements/path.hpp"








// bool cmp::__base_PatternElm::isTkn() const { return dynamic_cast<const PatternElm_Tkn*>(this); }
// bool cmp::__base_PatternElm::isElm() const { return dynamic_cast<const PatternElm_Elm*>(this); }

// cmp::Token     *cmp::__base_PatternElm::getAsTkn(){ return dynamic_cast<PatternElm_Tkn*>(this)->v; }
// cmp::__base_ST *cmp::__base_PatternElm::getAsElm(){ return dynamic_cast<PatternElm_Elm*>(this)->v; }





// Tree isType and asType functions
#define X(type, name) \
    const cmp::type *cmp::__base_ST::as##name() const { return dynamic_cast<const type*>(this); } \
    /**/  cmp::type *cmp::__base_ST::as##name()       { return dynamic_cast<      type*>(this); } \
    /**/       bool  cmp::__base_ST::is##name() const { return dynamic_cast<const type*>(this); } \
    /**/       bool  cmp::__base_ST::is##name()       { return dynamic_cast<      type*>(this); }
LIST_SOURCE_TREE_TYPES_NAMES
#undef X







// bool cmp::ST_Module   ::isChildAllowed(__base_ST* c) const { return !c->isStatement(); }
// bool cmp::ST_Namespace::isChildAllowed(__base_ST* c) const { return !c->isStatement(); }
// bool cmp::ST_Struct   ::isChildAllowed(__base_ST* c) const { return !(c->isStatement() || c->isFunction() || c->isOperator() || c->isVariable()); }
// bool cmp::ST_Enum     ::isChildAllowed(__base_ST* c) const { return !(c->isStatement() || c->isFunction() || c->isOperator() || c->isVariable()); }

// bool cmp::ST_Statement::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }
// bool cmp::ST_Function ::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }
// bool cmp::ST_Operator ::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }







//TODO check if these are all needed
std::string cmp::ST_Sub_Identifier::getCategoryName(bool plural) const { return plural ? "identifier"  : "identifiers"; } //TODO check if plural is needed
std::string cmp::ST_Sub_Keyword   ::getCategoryName(bool plural) const { return plural ? "keyword"  : "keywords"; } //TODO check if plural is needed

std::string cmp::ST_Sub_Type      ::getCategoryName(bool plural) const { return plural ? "type path"   : "type paths"; } //TODO check if plural is needed

std::string cmp::ST_Statement     ::getCategoryName(bool plural) const { return plural ? "statement"   : "statements"; } //TODO check if plural is needed

std::string cmp::ST_Module        ::getCategoryName(bool plural) const { return plural ? "module" : "modules"; } //TODO check if plural is needed
std::string cmp::ST_Struct        ::getCategoryName(bool plural) const { return plural ? "struct definition" : "struct definitions"; }

std::string cmp::ST_Import        ::getCategoryName(bool plural) const { return plural ? "import directive" : "import directives"; }
std::string cmp::ST_Export        ::getCategoryName(bool plural) const { return plural ? "export directive" : "export directives"; }

std::string cmp::ST_Variable      ::getCategoryName(bool plural) const { return plural ? "variable definition" : "variable definitions"; }
std::string cmp::ST_Function      ::getCategoryName(bool plural) const { return plural ? "function definition" : "function definitions"; }
std::string cmp::ST_Operator      ::getCategoryName(bool plural) const { return plural ? "operator definition" : "operator definitions"; }






// Pattern isType and asType functions
#define X(type, name) \
    const cmp::type *cmp::__base_Pattern::as##name() const { return dynamic_cast<const type*>(this); } \
    /**/  cmp::type *cmp::__base_Pattern::as##name()       { return dynamic_cast<      type*>(this); } \
    /**/       bool  cmp::__base_Pattern::is##name() const { return dynamic_cast<const type*>(this); } \
    /**/       bool  cmp::__base_Pattern::is##name()       { return dynamic_cast<      type*>(this); }
LIST_PATTERN_OPERATOR_TYPES_NAMES
LIST_PATTERN_BASES_TYPES_NAMES
LIST_PATTERN_TOKENS_TYPES_NAMES
#undef X




// Pattern singletons
namespace cmp {
    #define X(type, name) \
        void           *re::__internal_void_##name = nullptr; \
        __base_Pattern *re::__internal_base_##name = nullptr;
    LIST_PATTERN_ELM_TYPES_NAMES
    #undef X
}








cmp::Pattern_Elm_Module::Pattern_Elm_Module() : __base_Pattern_Composite(
    op::Loop(op::OneOf(
        re::Namespace(),
        re::Enum()
    ))
){}




cmp::__base_ST* cmp::Pattern_Elm_Module::generateData(std::vector<__base_ST*> const &results) const {
    ST_Module* r = new ST_Module;
    for(ulong i = 0; i < results.size(); ++i) {
        r->addChild(results[i]);
    }
    return r;
}