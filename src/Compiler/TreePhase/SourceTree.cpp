#include "SourceTree.hpp"
#include "PatternGenerators.hpp"








// Tree isType and asType functions
#define X(type, name) \
    const cmp::type *cmp::__base_ST::as##name() const { return dynamic_cast<const type*>(this); } \
    /**/  cmp::type *cmp::__base_ST::as##name()       { return dynamic_cast<      type*>(this); } \
    /**/       bool  cmp::__base_ST::is##name() const { return dynamic_cast<const type*>(this); } \
    /**/       bool  cmp::__base_ST::is##name()       { return dynamic_cast<      type*>(this); }
LIST_SOURCE_TREE_TYPES_NAMES
#undef X




//TODO check if these are all needed
std::string cmp::ST_Sub_Identifier::getCategoryName(bool plural) const { return plural ? "identifier"  : "identifiers"; } //TODO check if plural is needed
std::string cmp::ST_Sub_Keyword   ::getCategoryName(bool plural) const { return plural ? "keyword"  : "keywords"; } //TODO check if plural is needed

std::string cmp::ST_Statement     ::getCategoryName(bool plural) const { return plural ? "statement"   : "statements"; } //TODO check if plural is needed
