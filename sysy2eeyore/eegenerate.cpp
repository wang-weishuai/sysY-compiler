#define EEGEN

#include <list>
#include <string>
#include <iostream>
#include <unordered_map>
#include <map>
#include "eebasic.hpp"
#include "global.hpp"
#include "eegenerate.hpp"
#include "eeprint.hpp"
#include "parser.tab.hpp"

using std::list;
using std::string;
using std::cout;
using std::endl;

extern std::string tokenindex[1000];

list<LineOfEEyore> funclines;
list<string> temporaries;
list<string> labels;
list<PointerOfBaseAST> endovars;
list<string> glbvars;
list<LineOfEEyore> maininit;
int numofdec = 0;
int numofend = 0;
int numoftmp = 0;
int numoflabel = 0;

bool endpoint = false;

std::string sym2str(int symbol)//only for SingleAST.
{
	if(symbol == '-') return "-";
	if(symbol == '!') return "!";
}

static void InitializeTokenindex(int type)
{
	
	char tmp[2]; tmp[0] = type; tmp[1] = 0;
	for(auto i : tokenindex)
		i = tmp;
	
    tokenindex[LE] = "<=";
    tokenindex[GE] = ">=";
    tokenindex[EQ] = "==";
    tokenindex[NE] = "!=";
    tokenindex[AND] = "&&";
    tokenindex[OR] = "||";
    tokenindex[IDENT] = "identifier";
    tokenindex[INT_CONST] = "constant";
    tokenindex[CONST] = "const";
    tokenindex[INT] = "int";
    tokenindex[VOID] = "void";
    tokenindex[IF] = "if";
    tokenindex[ELSE] = "else";
    tokenindex[WHILE] = "while";
    tokenindex[BREAK] = "break";
    tokenindex[CONTINUE] =  "continue";
    tokenindex[RETURN] = "return";

}
static std::string Tokentostring(int type)
{
	if(type < IDENT || type > OR)
	{
		char tmp[2]; tmp[0] = type; tmp[1] = 0;
			return tmp;
	}
	InitializeTokenindex(type);
	return tokenindex[type];
}
static inline list<string>::iterator Newtemp()
{
	temporaries.push_front("t" + Encodemessage(numoftmp));
	numoftmp += 1;
	return temporaries.begin();
}

static inline list<string>::iterator Newlabel()
{
	labels.push_front("l" + Encodemessage(numoflabel));
	numoflabel += 1;	
	return labels.begin();
}

void BinaryAST::Generator()
{
	LineOfEEyore elt( EERecord::Uncond, std::move(truebranch));
	LineOfEEyore elf( EERecord::Uncond, std::move(falsebranch));

	/*class EEline ecalop;
	ecalop.sym[0] = addr;
	ecalop.sym[1] =lexp->addr;
	ecalop.sym[2] = rexp->addr;
	ecalop.op = Tokentostring(op);*/

	if(isconst)
	{
		addr = Encodemessage(cval);
		if(endpoint)
		{
			if(cval) 
				funclines.push_back(elt);
			else 
				funclines.push_back(elf);
		}
		return;
	}

	addr = *Newtemp();
	bool tmpsave = endpoint;
	endpoint = false;
	int iscalcop = (op == '+') + (op == '-') + (op == '*') + (op == '/') + (op == '%');
	int iscompop = (op == LE) + (op == GE) + (op == EQ) + (op == NE) + (op == '<') + (op == '>');
	if (iscalcop + iscompop)
	{
		lexp->Generator();
		rexp->Generator();
		LineOfEEyore ecalop1( EERecord::BinExpr, std::string(addr), std::move(lexp->addr), std::move(rexp->addr), Tokentostring(op));
		LineOfEEyore ecalop2( EERecord::Cond, std::string(addr), std::string("0"), std::string("!="), std::move(truebranch) );
		funclines.push_back(ecalop1);
		if(iscompop)
		{
			funclines.back().label = std::move(*Newlabel());
			funclines.back().num = std::move(*Newlabel());
			endpoint = tmpsave;
		}
		endpoint = tmpsave;
		if(endpoint)
		{
			funclines.push_back(ecalop2);
			funclines.push_back(elf);
		}
		return;
	}
	
	endpoint = tmpsave;
	int isorop = (op == OR) * (tmpsave == true);
	int isandop = (op == AND) * (tmpsave == true);
	std::string tmp; 
	if(isorop)
	{
		lexp->truebranch = truebranch;
		lexp->falsebranch = *Newlabel();
		tmp = lexp->falsebranch;
		
	}
	if(isandop)
	{
		lexp->truebranch = *Newlabel();		
		lexp->falsebranch = falsebranch;
		tmp = lexp->truebranch;
	}
	LineOfEEyore ellabelf( EERecord::Label, std::move(tmp) );
	rexp->truebranch = truebranch;
	rexp->falsebranch = falsebranch;
	lexp->Generator();
	funclines.push_back(ellabelf);
	rexp->Generator();
	return;
}

void SingleAST::Generator() 
{
	LineOfEEyore elt( EERecord::Uncond, std::move(truebranch));
	LineOfEEyore elf( EERecord::Uncond, std::move(falsebranch));

	if(isconst)
	{
		addr = Encodemessage(cval);
		if(endpoint)
		{
			if(cval) funclines.push_back(elt);
			else funclines.push_back(elf);
		}
		return;
	}

	bool tmpsave = endpoint;
	endpoint = false;
	exp->Generator();
	addr = exp->addr;	
	for(int i = 0;i < 3;++i)
	{
		int tmp = (op >> (i << 3)) & 0xFF;			

		if(tmp == '-' || tmp == '!') {
			auto p = Newtemp();
			LineOfEEyore el(EERecord::SingExpr, std::string(*p), std::string(addr), std::move((sym2str(tmp))));
			funclines.push_back(el);
			addr = *p;
		}
	
	}
	endpoint = tmpsave;
	if(endpoint)
	{
		LineOfEEyore el1(EERecord::Cond, std::string(addr), std::string("0"), std::string("!="), std::move(truebranch));
		funclines.push_back(el1);
		funclines.push_back(elf);
	}
}

void CallFunAST::Generator() 
{
	bool tmpsave = endpoint;
	endpoint = false;

	for(auto i: rparams)
		i->Generator();
	for(auto i: rparams)
		funclines.push_back(LineOfEEyore(EERecord::Param, std::move(i->addr)));
	if(((FuncSymtab *)sym)->isvoid)
		funclines.push_back(LineOfEEyore(EERecord::Voidcall, "f_" + (*(sym->strptr))));
	else
	{
		addr = *Newtemp();
		funclines.push_back(LineOfEEyore(EERecord::Asscall, std::string(addr), "f_" + (*(sym->strptr))));
	}

	endpoint = tmpsave;
	if(tmpsave)
	{
		funclines.push_back(LineOfEEyore(EERecord::Cond,std::string(addr), std::string("0"), std::string("!="), std::move(truebranch)));
		funclines.push_back(LineOfEEyore(EERecord::Uncond, std::move(falsebranch)));
	}
}

void LeftValueAST::Generator() 
{
	LineOfEEyore elt( EERecord::Uncond, std::move(truebranch));
	LineOfEEyore elf( EERecord::Uncond, std::move(falsebranch));

	if(isconst)
	{
		addr = Encodemessage(cval);
		if(endpoint)
		{
			if(cval) 
				funclines.push_back(elt);
			else 
				funclines.push_back(elf);
		}
		return;
	}

	bool tmpsave = endpoint;
	endpoint  = false;
	VarSymtab* symp = (VarSymtab *)sym;
	const int fsize = symp->tab_dimension.size();
	const int rsize = indices.size();
	int isok = (fsize == 1 )+ (rsize == 0);
	if(isok)
	{
		addr = symp->astptr->addr;
		endpoint = tmpsave;
		if(tmpsave)
		{
			funclines.push_back(LineOfEEyore(EERecord::Cond, std::string(addr), std::string("0"), std::string("!="), std::move(truebranch)));
			funclines.push_back(LineOfEEyore(EERecord::Uncond, std::move(falsebranch)));
		}
		return;
	}

	list<string>::iterator p1, p2, p3;
	p1 = Newtemp();
	indices[0]->Generator();
	funclines.push_back(LineOfEEyore(EERecord::BinExpr, std::string(*p1), std::move(indices[0]->addr), Encodemessage(symp->tab_dimension[fsize - 2]), std::string("*")));
	for(int i = 1; i < rsize; ++i)
	{
		p2 = Newtemp();
		p3 = Newtemp();
		indices[i]->Generator();
		funclines.push_back(LineOfEEyore(EERecord::BinExpr, std::string(*p2), std::move(indices[i]->addr), Encodemessage(symp->tab_dimension[fsize - 2 - i]), std::string("*")));
		funclines.push_back(LineOfEEyore(EERecord::BinExpr, std::string(*p3), std::move(*p1), std::move(*p2), std::string("+")));
		p1 = p3;
	}

	p2 = Newtemp();
	funclines.push_back(LineOfEEyore(EERecord::BinExpr, std::string(*p2), std::move(*p1), std::string("4"), std::string("*")));
	if(isleft)
	{
		endpoint = tmpsave;
		addr = sym->astptr->addr + "[" + *p2 + "]";
		return;
	}

	addr = *Newtemp();
	int flag = fsize - rsize - 1;
	if(flag) funclines.push_back(LineOfEEyore(EERecord::BinExpr, std::string(addr), std::string(symp->astptr->addr), std::move(*p2), std::string("+")));
	else funclines.push_back(LineOfEEyore(EERecord::RArr, std::string(addr), std::string(symp->astptr->addr), std::move(*p2)));

	endpoint = tmpsave;
	if(tmpsave)
	{
		funclines.push_back(LineOfEEyore(EERecord::Cond, std::string(addr), std::string("0"), std::string("!="), std::move(truebranch)));
		funclines.push_back(LineOfEEyore(EERecord::Uncond, std::move(falsebranch)));
	}

}

void FunStmtAST::Generator()
{
	for(auto i:clauses)
		i->Generator();
}

void DefVarAST::Generator()
{
	endovars.push_back(this);
	VarSymtab * symp = (VarSymtab *)sym;

	if(symp->tab_dimension.size() == 1)
	{
		if(!init.empty()) 
		{
			init[0]->Generator();
			funclines.push_back(LineOfEEyore(EERecord::Copy, std::string(addr),  std::move(init[0]->addr)));
		}
		return;
	}

	for(auto i:init)
	{
		i.second->Generator();
		funclines.push_back(LineOfEEyore(EERecord::LArr, std::string(addr), Encodemessage(i.first * 4), std::move(i.second->addr)));
	}
}


void DefFunAST::Generator()
{
	clearall();
	funclines.push_back(LineOfEEyore(EERecord::Header, std::string(addr),Encodemessage(fparams.size())));

	if(*(sym->strptr) == "main")
		funclines.splice(funclines.end(), maininit);
	
	body->Generator();
	auto q = ++funclines.begin();
	for(auto i: endovars)
	{
		DefVarAST *tmpp = dynamic_cast<DefVarAST *>(i);
		VarSymtab *symp = dynamic_cast<VarSymtab *>(tmpp->sym);
		if(symp->tab_dimension.size() == 1)
			funclines.insert(q,LineOfEEyore(EERecord::DefVar, std::string(tmpp->addr)));
		else{ // an array
			funclines.insert(q,LineOfEEyore(EERecord::DefVar, std::string(tmpp->addr), Encodemessage(symp->tab_dimension.back() * 4)));
		}
	}
	for(auto &i: temporaries)
		funclines.insert(q, LineOfEEyore(EERecord::DefVar, std::move(i)));
	if(((FuncSymtab *)sym)->isvoid)
		funclines.push_back(LineOfEEyore(EERecord::Voidret));
	else
		funclines.push_back(LineOfEEyore(EERecord::Ret, std::string("0")));
	funclines.push_back(LineOfEEyore(EERecord::End, std::string(addr)));

	eelines.splice(eelines.end(), funclines);
	clearall();
}




void VoluAST::Generator() 
{
	((LeftValueAST *)lval)->isleft = true;
	lval->Generator();
	exp->Generator();
	int len = lval->addr.size();
	int flag = lval->addr.back() - ']'; 
	if(flag) funclines.push_back(LineOfEEyore(EERecord::Copy, std::move(lval->addr), std::move(exp->addr)));
	else funclines.push_back(LineOfEEyore(EERecord::LArr,lval->addr.substr(0, lval->addr.find("[")), lval->addr.substr(lval->addr.find("[") + 1, len - lval->addr.find("[") - 2), std::move(exp->addr)));
	
}

void IfAST::Generator() 
{
	std::string tmptrue = *Newlabel();
	std::string tmpfalse = *Newlabel();	
	cond->truebranch = tmptrue;
	cond->falsebranch = tmpfalse;
	next = *Newlabel();
	endpoint = true;
	cond->Generator();
	endpoint = false;
	LineOfEEyore e(EERecord::Label, std::move(tmptrue));
	funclines.push_back(e);
	if(then) then->Generator();
	funclines.push_back(LineOfEEyore(EERecord::Uncond, std::string(next)));
	funclines.push_back(LineOfEEyore(EERecord::Label, std::move(tmpfalse)));
	if(els) els->Generator();
	funclines.push_back(LineOfEEyore(EERecord::Label, std::move(next)));
}

void WhileAST::Generator() 
{
	begin = *Newlabel();
	cond->truebranch = *Newlabel();
	std::string tmptrue = cond->truebranch;
	cond->falsebranch = *Newlabel();
	next = cond->falsebranch;
	funclines.push_back(LineOfEEyore(EERecord::Label, std::string(begin)));
	endpoint = true;
	cond->Generator();
	endpoint = false;
	funclines.push_back(LineOfEEyore(EERecord::Label, std::move(tmptrue)));
	if(body) body->Generator();
	funclines.push_back(LineOfEEyore(EERecord::Uncond, std::move(begin)));
	funclines.push_back(LineOfEEyore(EERecord::Label, std::move(next)));
}

void BreakAST::Generator() 
{
	funclines.push_back(LineOfEEyore(EERecord::Uncond, std::string(back->next)));
}

void ContinueAST::Generator() 
{
	funclines.push_back(LineOfEEyore(EERecord::Uncond, std::string(back->begin)));
}

void RetAST::Generator() 
{
	if(exp)
	{
		exp->Generator();
		funclines.push_back(LineOfEEyore(EERecord::Ret, std::move(exp->addr)));
	}
	else 
		funclines.push_back(LineOfEEyore(EERecord::Voidret));
}

void Treatmain(PointerOfBaseAST root)
{
	std::vector<PointerOfBaseAST> pp = ((FunStmtAST *)root)->clauses;
	for(std::vector<PointerOfBaseAST>::iterator i = pp.begin(); i != pp.end(); i++){
		int isdecl = ((*i)->type == ASTType::DefVar);
		if(isdecl)
		{
			DefVarAST * p = (DefVarAST *)(*i);
			VarSymtab * symp = (VarSymtab *)(p -> sym);
			if(symp->tab_dimension.size() == 1)
			{
				std::string tmpstr = p -> addr;
				glbvars.emplace_back(tmpstr);
				if(!p->init.size()) continue;
				p->init[0]->Generator();
			}
		}
	}
	glbvars.splice(glbvars.end(), temporaries);
	for(auto &i: glbvars){
		if ('0' <= i[0] && i[0] <= '9'){
			int t = i.find(" ");
			eelines.emplace_back(EERecord::DefVar, i.substr(t + 1), i.substr(0, t));
		}
		else
			eelines.emplace_back(EERecord::DefVar, std::move(i));
	}

	glbvars.clear();




}

void TraverseAST(PointerOfBaseAST root)
{
	if(root)
 	for(auto i: ((FunStmtAST *)root)->clauses)
		if(i->type == ASTType::DefFunc) i->Generator();
}