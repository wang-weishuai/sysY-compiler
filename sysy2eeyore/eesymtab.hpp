#pragma once
#ifndef EESYM
#define EESYM

#include "eebasic.hpp"
#include "global.hpp"
#include <string>
#include <vector>
#include <unordered_set>
class Token;
class BaseAST;
class BaseSymtab;
class EqualSymtab;
class HashSymtab;
class SYMTab;
using PointerOfBaseAST = BaseAST*;
using PointerOfBaseSymtab = BaseSymtab *;
using PointerOfSTptr = SYMTab *;

extern std::hash<std::string> hashstr;
extern PointerOfSTptr const glbst; 

enum class SYMType {Var, Fun};

class BaseSymtab {
public:
	BaseSymtab(std::list<std::string>::iterator p, SYMType type_, PointerOfBaseAST astptr_): strptr(p), type(type_), astptr(astptr_), fingerprint(hashstr(*p)) {};
	virtual ~BaseSymtab() {};
	std::list<std::string>::iterator strptr;
	SYMType type; 
	PointerOfBaseAST astptr; 
	const size_t fingerprint;
};

class VarSymtab: public BaseSymtab {
public:
	VarSymtab(bool isconst_, Token* p, PointerOfBaseAST astptr_);
	~VarSymtab() {};
	void Setarraytype(std::vector<int>);
	const bool isconst;
	std::vector<int> tab_dimension;

};

class FuncSymtab: public BaseSymtab {
public:
	FuncSymtab(bool isvoid_, Token* p, PointerOfBaseAST astptr_);
	~FuncSymtab() {};
	void Insertfparams(PointerOfBaseSymtab);
	const bool isvoid;
	std::vector<PointerOfBaseSymtab> fparams; 
};

class EqualSymtab{
public:
	bool operator () (PointerOfBaseSymtab p, PointerOfBaseSymtab q) const {
		return *(p->strptr) == *(q->strptr);
	}
};

class HashSymtab{
public:
	size_t operator () (PointerOfBaseSymtab p) const {return p->fingerprint;}
};

class SYMTab{
public:
	//SYMTab(int bno_, PointerOfSTptr fth = nullptr): father(fth), bno(bno_) {}
	SYMTab(PointerOfSTptr fth = nullptr): father(fth) {}

	~SYMTab(); 
	std::unordered_set<PointerOfBaseSymtab, HashSymtab, EqualSymtab> symtab;
	std::vector<PointerOfSTptr> child;
	PointerOfSTptr father;
	int bno;
};



PointerOfBaseSymtab NewSymtab(int mode, bool ismode, Token* p, PointerOfBaseAST q);

void Pushsymtab();
void Popsymtab();
void Insertsymentry(PointerOfBaseSymtab);
PointerOfBaseSymtab Lookupstring(std::list<std::string>::iterator p);
PointerOfBaseSymtab Checkifdeclared(PointerOfBaseSymtab p);

#endif