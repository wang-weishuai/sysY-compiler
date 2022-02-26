#pragma once
#ifndef EEBASIC
#define EEBASIC

#include "eesymtab.hpp"
#include "eeprint.hpp"
#include <vector>
#include <map>
#include <list>
#include "global.hpp"

class BaseAST;
class ArrayTree;
class ArrayAST;
class DefFunAST;
class LeftValueAST;
class BaseSymtab;
class EqualSymtab;
class HashSymtab;
class SYMTab;
using PointerOfBaseAST = BaseAST *;
using PointerOfArrayTree = ArrayTree *;
using PointerOfArrarAST = ArrayAST *;
using PointerOfSTptr = SYMTab *;
using PointerOfBaseSymtab = BaseSymtab *;
extern PointerOfBaseAST mainptr;
using std::list;
using std::string;
extern list<LineOfEEyore> funclines;
extern list<string> temporaries;
extern list<string> labels;
extern list<PointerOfBaseAST> endovars;
extern list<string> glbvars;
extern list<LineOfEEyore> maininit;
enum class ASTType
{
	Basic, 
	SingExpr, BinExpr, 
	CallFunc,DefFunc,FunStatement,  
	DefVar , Voluation , Lvalue ,  If, 
	While, Break, Continue, Return, ArrTree, Array
};
class Token{
public:
	Token(int type_)
	{
		type = type_;
	}
	Token(int val_, int type_)
	{
		val = val_;
		type = type_;
	}
	Token(const char *p, int type_);
	Token(std::string p, int type_);

	int val; 
	std::list<std::string>::iterator strptr; 
	int type; 
	int lno, bgn, end; 
};

extern std::list<PointerOfBaseAST> whilestk;
extern PointerOfBaseAST curfundef;
extern std::string tokenindex[1000];
/*There is something wrong when linking*/
class BaseAST{
public:
	BaseAST(ASTType type_ = ASTType::Basic){type = type_;}
	virtual ~BaseAST() {};
	virtual bool Isconst() {return false;}
	virtual int Cval(){return 0;}
	virtual bool Isanumber(){return false;}	
	virtual void Eval() {;}
	virtual void Generator(){;}

	ASTType type;
	std::string addr;
	std::string truebranch;
	std::string falsebranch;
	std::string next;
	std::string begin;
};


class SingleAST: public BaseAST{
public:
	SingleAST(PointerOfBaseAST ch, Token* op): BaseAST(ASTType::SingExpr), isconst(false), cval(0), exp(ch), op(op->type), isevaled(false) {}	
	~SingleAST() {if(exp) {delete exp; exp = nullptr;}}	
	void Coalesce(Token* p)//May still have some error.
	{
	
			int ptype = p -> type;
			if(op == '+') op = ptype;
  			if(op == '-') 
			{
				if(ptype == '+')
				{
					op = '-';
				}
				else if(ptype == '-')
				{
					op = '+';
				}
				else if(ptype == '!')
				{
					;
				}
			}
			if(op == '!') 
			{
				if(ptype == '+')
				{
					op = '!';
				}
				else if(ptype == '-')
				{
					op = '!';
				}
				else if(ptype == '!')
				{
					op = '+';
				}
			}
		
	}

	void Eval() override
	{
		if(isevaled + isconst == 0)
		{
			exp->Eval();
			if(exp->Isconst())
			{
				isconst = true;
				cval = exp->Cval();
				
				delete exp;
				exp = nullptr;
		
				for(int i = 0;i < 3;++i)
				{
					int tmp = (op >> (i << 3)) & 0xFF;
					if(tmp == '-') cval = -cval;
					if(tmp == '!') cval = !cval;
				}
				op = 0;
			}
			isevaled = true;
			return;
		}
	}
	bool Isconst() override{return isconst;}
	bool Isanumber() override
	{
		if(isconst) return true;
 		if(exp && !exp->Isanumber()) return false;
 		return true;
	}
	int Cval() override {return cval;}

	PointerOfBaseAST Exp() {return exp;}
	int Op() {return op;}

	void Generator() override;

	bool isconst; 
	bool isevaled; 
	int op;
	int cval;
	PointerOfBaseAST exp;
};

class BinaryAST: public BaseAST{
public:
	BinaryAST(Token* p): BaseAST(ASTType::BinExpr), isconst(true), cval(p->val), lexp(nullptr), rexp(nullptr), op(0), isevaled(false)  {};
	BinaryAST(PointerOfBaseAST lch, PointerOfBaseAST rch, Token* op): BaseAST(ASTType::BinExpr), isconst(false), cval(0), lexp(lch), rexp(rch), op(op->type), isevaled(false) {};	
	~BinaryAST() {if(lexp) {delete lexp; lexp = nullptr;} if(rexp) {delete rexp; rexp = nullptr;}}

	void Eval() override;
	bool Isconst() override {return isconst;}
	int Cval() override {return cval;}
	bool Isanumber() override
	{
		if(isconst) return true;
 		if( (lexp || rexp ) && (!lexp->Isanumber())) return false;
 		if(rexp && !rexp->Isanumber()) return false;
 		return true;
	}

	PointerOfBaseAST Lexp() {return lexp;}
	PointerOfBaseAST Rexp() {return rexp;}
	int Op() {return op;}
	
	void Generator() override;

	bool isconst; 
	bool isevaled;
	int op;
	int cval;
	PointerOfBaseAST lexp, rexp;
};

class CallFunAST: public BaseAST{
public:

	CallFunAST(Token* p);
	~CallFunAST();
	bool Checkparamlistsize();
	bool Isanumber() override;
	bool Isconst() override{return false;}
	void Eval() override {return;}
	void Generator() override;
	PointerOfBaseSymtab sym;
	std::vector<PointerOfBaseAST> rparams;
};

class LeftValueAST: public BaseAST{
public:
	LeftValueAST(Token* p, PointerOfBaseAST q);
	~LeftValueAST();
	bool Isanumber() override;
	void Eval() override;
	bool Isconst() override{return isconst;}
	int Cval() override{return cval;}
	void Generator() override;
	PointerOfBaseSymtab sym;
	std::vector<PointerOfBaseAST> indices;
	bool isconst;
	bool isevaled;
	int cval;
	bool isleft;
};

class FunStmtAST: public BaseAST{
public:
	FunStmtAST(): BaseAST(ASTType::FunStatement){}
	~FunStmtAST() 
	{
		for(auto i: clauses) 
			if(i) delete i; 
		clauses.clear();
	}
	void Insertclause(PointerOfBaseAST p) {if(p) clauses.push_back(p);}
	void Insertclause(std::vector<PointerOfBaseAST> p) {for(auto i: p) clauses.push_back(i);}
	void Generator() override;
	std::vector<PointerOfBaseAST> clauses;
};

class DefVarAST: public BaseAST{
public:
	DefVarAST():BaseAST(ASTType::DefVar), sym(nullptr) {}
	~DefVarAST();
	PointerOfBaseAST Getinitvalue(int index);
	void Generator() override;
	PointerOfBaseSymtab sym;
	std::map<int, PointerOfBaseAST> init;
};

class DefFunAST: public BaseAST{
public:
	DefFunAST():BaseAST(ASTType::DefFunc), sym(nullptr), body(nullptr) {}
	~DefFunAST()
	{
		 for(auto i:fparams) delete i;
 			delete body;
	} 	
	void clearall()
	{
		funclines.clear();
		temporaries.clear();
		labels.clear();
		endovars.clear();
	}
	void Generator() override;
	PointerOfBaseSymtab sym;
	std::vector<PointerOfBaseAST> fparams;
	PointerOfBaseAST body;
};

class VoluAST: public BaseAST{
public:
	VoluAST(): BaseAST(ASTType::Voluation), lval(nullptr), exp(nullptr) {}
	~VoluAST() 
	{
		if(lval) 
			delete lval; 
		if(exp) 
			delete exp; 
		lval = exp = nullptr;
	}
	void Generator() override;
	PointerOfBaseAST lval, exp;
};

class IfAST: public BaseAST{
public:
	IfAST():BaseAST(ASTType::If), cond(nullptr), then(nullptr), els(nullptr) {}
	~IfAST() 
	{
		if(cond)
			delete cond;
		if(then) 
			delete then;
		if(els) 
			delete els;
		cond = then = els = nullptr;
	}
	void Generator() override;

	PointerOfBaseAST cond, then, els;
};

class WhileAST: public BaseAST{
public:
	WhileAST():BaseAST(ASTType::While), cond(nullptr), body(nullptr) {}
	~WhileAST() 
	{
		if(cond) 
			delete cond; 
		if(body)
			delete body; 
		cond = body = nullptr;
	}

	void Generator() override;

	PointerOfBaseAST cond, body;
};

class BreakAST: public BaseAST{
public:
	BreakAST():BaseAST(ASTType::Break), back(whilestk.back()) {}
	~BreakAST() {;}

	void Generator() override;
	
	PointerOfBaseAST back;
};

class ContinueAST: public BaseAST{
public:
	ContinueAST():BaseAST( ASTType::Continue), back(whilestk.back()) {}
	~ContinueAST() {;}

	void Generator() override;

	PointerOfBaseAST back;
};

class RetAST: public BaseAST{
public:
	RetAST():BaseAST(ASTType::Return), back(curfundef), exp(nullptr) {}
	~RetAST() 
	{
		if(exp) 
			delete exp; 
		exp = nullptr;
	}

	void Generator() override;

	PointerOfBaseAST back;
	PointerOfBaseAST exp;
};

class ArrayAST: public BaseAST{
public:
	ArrayAST():BaseAST(ASTType::Array) {}
	~ArrayAST() {;} 

	void Insertastlist(PointerOfBaseAST p);
	std::vector<int> Getrawarraytype(); 
	std::vector<PointerOfBaseAST> &Astlist() {return astlist;}
	std::vector<PointerOfBaseAST> astlist;
};


class ArrayTree: public BaseAST {
public:
	ArrayTree():BaseAST(ASTType::ArrTree), astptr(nullptr) {} 
	ArrayTree(Token* lbr, Token* rbr):BaseAST(ASTType::ArrTree), astptr(nullptr) {}
	ArrayTree(PointerOfBaseAST astptr):BaseAST(ASTType::ArrTree), astptr(astptr) {}	
	~ArrayTree() 
	{
		if(!astptr) 
		{
			for(auto p: sublist) 
				delete p;
			sublist.clear();
		}
	}
	
	std::map<int, BaseAST*>  Flatten(std::vector <int> dim)
	{
		std::map<int, BaseAST* > ret; 
		std::vector<int> savedim = dim; // Copy constructor
		_Flatten_(savedim, ret, 0, savedim.back());
		return ret;
	}
	void _Flatten_(std::vector<int> dim, std::map<int, BaseAST* > res, int ind, int upper){
	if( !astptr && sublist.empty() ) return;
	if(dim.size() == 1) {
		if(ind >= upper) return;
		if(astptr) res[ind] = astptr;
		return;
	}

	const int tot = dim.back();
	dim.pop_back();
	const int no = dim.back();

	for(auto p: sublist){
		if( !dynamic_cast<ArrayTree*>(p)->astptr ){
			dynamic_cast<ArrayTree*>(p)->_Flatten_(dim, res, ind, upper);
			ind += no;
			if(ind >= upper) return;
		}
		else {
			if(ind >= upper) return;
			res[ind] = dynamic_cast<ArrayTree*>(p)->astptr;
			ind += 1;
		}
	}
	dim.push_back(tot);
	return;
	}


	void Insertsublist(PointerOfBaseAST p);
	PointerOfBaseAST astptr; // Pointer to AST. An expression is stored there, which is hopefully a scalar.
	std::vector<PointerOfBaseAST> sublist; // Pointer to a sub-list.
};



#endif