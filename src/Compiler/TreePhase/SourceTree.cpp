#include "SourceTree.hpp"








bool cmp::__base_ST::isType     () const { return dynamic_cast<const ST_Sub_Type *>(this); }
bool cmp::__base_ST::isPath     () const { return dynamic_cast<const ST_Sub_Path *>(this); }

bool cmp::__base_ST::isStatement() const { return dynamic_cast<const ST_Statement*>(this); }

bool cmp::__base_ST::isModule   () const { return dynamic_cast<const ST_Module   *>(this); }
bool cmp::__base_ST::isNamespace() const { return dynamic_cast<const ST_Namespace*>(this); }
bool cmp::__base_ST::isStruct   () const { return dynamic_cast<const ST_Struct   *>(this); }
bool cmp::__base_ST::isEnum     () const { return dynamic_cast<const ST_Enum     *>(this); }
bool cmp::__base_ST::isAlias    () const { return dynamic_cast<const ST_Alias    *>(this); }

bool cmp::__base_ST::isImport   () const { return dynamic_cast<const ST_Import   *>(this); }
bool cmp::__base_ST::isExport   () const { return dynamic_cast<const ST_Export   *>(this); }

bool cmp::__base_ST::isVariable () const { return dynamic_cast<const ST_Variable *>(this); }
bool cmp::__base_ST::isFunction () const { return dynamic_cast<const ST_Function *>(this); }
bool cmp::__base_ST::isOperator () const { return dynamic_cast<const ST_Operator *>(this); }









bool cmp::ST_Module   ::isChildAllowed(__base_ST* c) const { return !c->isStatement(); }
bool cmp::ST_Namespace::isChildAllowed(__base_ST* c) const { return !c->isStatement(); }
bool cmp::ST_Struct   ::isChildAllowed(__base_ST* c) const { return !(c->isStatement() || c->isFunction() || c->isOperator() || c->isVariable()); }
bool cmp::ST_Enum     ::isChildAllowed(__base_ST* c) const { return !(c->isStatement() || c->isFunction() || c->isOperator() || c->isVariable()); }

bool cmp::ST_Statement::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }
bool cmp::ST_Function ::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }
bool cmp::ST_Operator ::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }








std::string cmp::ST_Sub_Type ::getCategoryName(bool plural) const { return plural ? "type path"   : "type paths"; } //TODO check if plural is needed
std::string cmp::ST_Sub_Path ::getCategoryName(bool plural) const { return plural ? "symbol path" : "symbol paths"; } //TODO check if plural is needed
std::string cmp::ST_Statement::getCategoryName(bool plural) const { return plural ? "statement"   : "statements"; } //TODO check if plural is needed

std::string cmp::ST_Module   ::getCategoryName(bool plural) const { return plural ? "module" : "modules"; } //TODO check if plural is needed
std::string cmp::ST_Namespace::getCategoryName(bool plural) const { return plural ? "namespace definition" : "namespace definitions"; }
std::string cmp::ST_Struct   ::getCategoryName(bool plural) const { return plural ? "struct definition" : "struct definitions"; }
std::string cmp::ST_Enum     ::getCategoryName(bool plural) const { return plural ? "enum definition" : "enum definitions"; }
std::string cmp::ST_Alias    ::getCategoryName(bool plural) const { return plural ? "symbol alias definition" : "symbol alias definitions"; }

std::string cmp::ST_Import   ::getCategoryName(bool plural) const { return plural ? "import directive" : "import directives"; }
std::string cmp::ST_Export   ::getCategoryName(bool plural) const { return plural ? "export directive" : "export directives"; }

std::string cmp::ST_Variable ::getCategoryName(bool plural) const { return plural ? "variable definition" : "variable definitions"; }
std::string cmp::ST_Function ::getCategoryName(bool plural) const { return plural ? "function definition" : "function definitions"; }
std::string cmp::ST_Operator ::getCategoryName(bool plural) const { return plural ? "operator definition" : "operator definitions"; }