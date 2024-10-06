#include "SourceTree.hpp"





bool cmp::__base_ST::isType     () const { return dynamic_cast<const ST_Sub_Type     *>(this); }
bool cmp::__base_ST::isPath     () const { return dynamic_cast<const ST_Sub_Path     *>(this); }

// bool cmp::__base_ST::isExpr     () const { return dynamic_cast<const ST_Expr     *>(this); }

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






bool cmp::ST_Sub_Type     ::isChildAllowed(__base_ST* c) const { return false; }
bool cmp::ST_Sub_Path     ::isChildAllowed(__base_ST* c) const { return false; }
// bool cmp::ST_Sub_Expr     ::isChildAllowed(__base_ST* c) const { return false; }

bool cmp::ST_Module   ::isChildAllowed(__base_ST* c) const { return !c->isExpr(); }
bool cmp::ST_Namespace::isChildAllowed(__base_ST* c) const { return !c->isExpr(); }
bool cmp::ST_Struct   ::isChildAllowed(__base_ST* c) const { return !(c->isExpr() || c->isFunction() || c->isOperator() || c->isVariable()); }
bool cmp::ST_Enum     ::isChildAllowed(__base_ST* c) const { return !(c->isExpr() || c->isFunction() || c->isOperator() || c->isVariable()); }
bool cmp::ST_Alias    ::isChildAllowed(__base_ST* c) const { return false; }

bool cmp::ST_Import   ::isChildAllowed(__base_ST* c) const { return false; }
bool cmp::ST_Export   ::isChildAllowed(__base_ST* c) const { return false; }

bool cmp::ST_Variable ::isChildAllowed(__base_ST* c) const { return false; }
bool cmp::ST_Function ::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }
bool cmp::ST_Operator ::isChildAllowed(__base_ST* c) const { return !(c->isFunction() || c->isOperator()); }