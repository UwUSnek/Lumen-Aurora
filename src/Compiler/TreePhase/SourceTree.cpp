#include "SourceTree.hpp"








// bool cmp::__base_PatternElm::isTkn() const { return dynamic_cast<const PatternElm_Tkn*>(this); }
// bool cmp::__base_PatternElm::isElm() const { return dynamic_cast<const PatternElm_Elm*>(this); }

// cmp::Token     *cmp::__base_PatternElm::getAsTkn(){ return dynamic_cast<PatternElm_Tkn*>(this)->v; }
// cmp::__base_ST *cmp::__base_PatternElm::getAsElm(){ return dynamic_cast<PatternElm_Elm*>(this)->v; }






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
std::string cmp::ST_Sub_Path      ::getCategoryName(bool plural) const { return plural ? "symbol path" : "symbol paths"; } //TODO check if plural is needed
std::string cmp::ST_Statement     ::getCategoryName(bool plural) const { return plural ? "statement"   : "statements"; } //TODO check if plural is needed

std::string cmp::ST_Module        ::getCategoryName(bool plural) const { return plural ? "module" : "modules"; } //TODO check if plural is needed
std::string cmp::ST_Namespace     ::getCategoryName(bool plural) const { return plural ? "namespace definition" : "namespace definitions"; }
std::string cmp::ST_Struct        ::getCategoryName(bool plural) const { return plural ? "struct definition" : "struct definitions"; }
std::string cmp::ST_Enum          ::getCategoryName(bool plural) const { return plural ? "enum definition" : "enum definitions"; }
std::string cmp::ST_Alias         ::getCategoryName(bool plural) const { return plural ? "symbol alias definition" : "symbol alias definitions"; }

std::string cmp::ST_Import        ::getCategoryName(bool plural) const { return plural ? "import directive" : "import directives"; }
std::string cmp::ST_Export        ::getCategoryName(bool plural) const { return plural ? "export directive" : "export directives"; }

std::string cmp::ST_Variable      ::getCategoryName(bool plural) const { return plural ? "variable definition" : "variable definitions"; }
std::string cmp::ST_Function      ::getCategoryName(bool plural) const { return plural ? "function definition" : "function definitions"; }
std::string cmp::ST_Operator      ::getCategoryName(bool plural) const { return plural ? "operator definition" : "operator definitions"; }







#define X(type, name) \
    const cmp::type *cmp::__base_Pattern::as##name() const { return dynamic_cast<const type*>(this); } \
    /**/  cmp::type *cmp::__base_Pattern::as##name()       { return dynamic_cast<      type*>(this); } \
    /**/       bool  cmp::__base_Pattern::is##name() const { return dynamic_cast<const type*>(this); } \
    /**/       bool  cmp::__base_Pattern::is##name()       { return dynamic_cast<      type*>(this); }
LIST_PATTERN_BASES_TYPES_NAMES
#undef X