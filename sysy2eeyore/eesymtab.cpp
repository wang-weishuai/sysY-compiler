#include "eesymtab.hpp"
#include "global.hpp"
#include "eebasic.hpp"
#include <iostream>
PointerOfSTptr const glbst = new SYMTab();
static PointerOfSTptr crrst = glbst;
class Token;
std::hash<std::string> hashstr;
SYMTab::~SYMTab()
{	
	for(auto p: child) delete p;
	for(auto p: symtab) delete p;
}

VarSymtab::VarSymtab(bool isconst_, Token* p, PointerOfBaseAST astptr_): BaseSymtab(p->strptr, SYMType::Var, astptr_), isconst(isconst_) {};

void VarSymtab::Setarraytype(std::vector<int> a) {tab_dimension = a;}

FuncSymtab::FuncSymtab(bool isvoid_, Token* p, PointerOfBaseAST astptr_): BaseSymtab(p->strptr, SYMType::Fun, astptr_), isvoid(isvoid_) {};

void FuncSymtab::Insertfparams(PointerOfBaseSymtab p)
{
	fparams.push_back(p);
}


void Pushsymtab()
{
	PointerOfSTptr p = new SYMTab(crrst);
	crrst->child.push_back(p);
	crrst = p;
}

void Popsymtab()
{
	crrst = crrst->father;
}

void Insertsymentry(PointerOfBaseSymtab p)
{
	if(p) crrst->symtab.insert(p);
}

PointerOfBaseSymtab Lookupstring(std::list<std::string>::iterator p)
{
	PointerOfBaseSymtab tmp = new BaseSymtab(p, SYMType::Var, nullptr);
	int flg = 1;
	for(auto q = crrst; q; q = q->father){
		auto k = q->symtab.find(tmp);
		if(k != q->symtab.end()) {
			flg = 0;
			delete tmp;
			return *k;
		}
	}
	if(flg)
		delete tmp;
	return nullptr;
}

PointerOfBaseSymtab Checkifdeclared(PointerOfBaseSymtab p){
	auto k = crrst->symtab.find(p);
	if(k != crrst->symtab.end()) return *k;
	return nullptr;
}

PointerOfBaseSymtab NewSymtab(int mode, bool ismode, Token* p, PointerOfBaseAST q)
{
	PointerOfBaseSymtab newsym;
	if(mode == 0)
		 newsym = new FuncSymtab(ismode, p, q);
	if(mode == 1)
		 newsym = new VarSymtab(ismode, p, q);

	PointerOfBaseSymtab prev = Checkifdeclared(newsym);
	
	if(prev) 
		return nullptr;
	return newsym;
}