%{
#include "global.hpp"
#include "eebasic.hpp"
#include "eesymtab.hpp"
#include "eegenerate.hpp"
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

// These are intricacies when putting lex and yacc together.
// Just neglect these minor points, or nasty here.
extern FILE *yyin, *yyout;
int yylex(void);
void yyerror(PointerOfBaseAST *, const char *){};

%}

%union{
	Token* tkptr;
	BaseAST* astptr;
}

%define parse.error verbose
%define parse.lac full
%parse-param {PointerOfBaseAST *root}

%token <tkptr> IDENT "identifier"
%token <tkptr> INT_CONST "constant"
%token <tkptr> CONST "const" INT "int" VOID "void" IF "if" ELSE "else" WHILE "while" BREAK "break" CONTINUE "continue" RETURN "return"
%token <tkptr> LE "<=" GE ">=" EQ "==" NE "!=" AND "&&" OR "||"
%token <tkptr> '+' '-' '!' '*' '/' '%' '<' '>' ',' ';' '[' ']' '=' '{' '}' '(' ')'

%type <tkptr> UnaryOp
%type <astptr> CompUnit ConstDef VarDef FuncFParam FuncDef FunStatement Stmt Exp Cond LVal PrimaryExp UnaryExp MulExp AddExp RelExp EqExp LAndExp LOrExp ConstExp
%type <astptr> DefVar ConstDefArr ConstDecl VarDefArr VarDecl FuncFParams BlockItemArr ExpArr FuncRParams ConstArr
%type <astptr> ConstInitValArr ConstInitVal InitValArr InitVal FuncDefHead BlockStart FuncDefproto WhileHead CompHead
%%
CompUnit	:	CompUnit DefVar
	{
		FunStmtAST *p = (FunStmtAST *)$1;
		p->Insertclause(((PointerOfArrarAST)$2)->astlist); //insertclauses
		$$ = p;
	}
			|	CompUnit FuncDef
	{
		((FunStmtAST *)$1)->Insertclause($2);
		$$ = $1;
	}
			|	CompUnit ';'
	{
		$$ = $1;
	}
			|	CompHead DefVar
	{
		FunStmtAST *p = new FunStmtAST;
		p->Insertclause(((PointerOfArrarAST)$2)->astlist);//insertclauses
		$$ = p;
		*root = p;
	}
			|	CompHead FuncDef
	{
		FunStmtAST *p = new FunStmtAST;
		p->Insertclause($2);
		$$ = p;
		*root = p;
	}
			|	CompHead ';'
	{
		*root = $$;
		delete $2;
	}
			;
CompHead	:
	{
		std::string arrr[14]  = {"getint","getch","getarray","a","putint","a","putch","a","putarray","a","b","starttime","stoptime"};
		int flag[13] = {2,2,1,0,1,0,1,0,1,0,0,2,2};
		Token* p;PointerOfBaseSymtab q;Token* r;PointerOfBaseSymtab s;
			for(int i = 0; i < 13; ++i)
			{
				if(flag[i])
				{
					p = new Token(arrr[i], IDENT);
					if(i < 3)
						q = NewSymtab(0,false, p, nullptr);
					else
						q = NewSymtab(0,true, p, nullptr);
					Insertsymentry(q);
					if(!(flag[i] % 2))
						delete p;
				}
				else
				{
					(i == 3 || i == 5 || i == 7 || i == 9) && ([&](){Pushsymtab(); return 1;}());
					r = new Token(arrr[i], IDENT);
					s = NewSymtab(1,false, r, nullptr);
					Insertsymentry(s);
					((VarSymtab*)s)->tab_dimension.push_back(1);
					(i == 3 || i == 10) && ([&](){ ((VarSymtab*)s)->tab_dimension.push_back(0);return 1;}());
					((FuncSymtab *)q)->Insertfparams(s);
					(i == 3 || i == 5 || i == 7) && ([&](){Popsymtab();return 1;}());
					delete r;
					(i == 10) && ([&](){Popsymtab();return 1;}());
					(i != 9) && ([&](){delete p;return 1;}());
				}
			}
		

	}
			;
DefVar		:	ConstDecl
			|	VarDecl
			;
ConstDefArr	:	ConstDefArr ',' ConstDef
	{
		PointerOfArrarAST p = (PointerOfArrarAST)$1;
		p->Insertastlist($3);
		$$ = p;
	}
			|	ConstDef
	{
		PointerOfArrarAST p = new ArrayAST;
		p->Insertastlist($1);
		$$ = p;
	}
			;
ConstDecl	:	CONST INT ConstDefArr ';'
	{
	    $$ = $3;
	}
			;
ConstArr	:	ConstArr '[' ConstExp ']'
	{
		((PointerOfArrarAST)$1)->Insertastlist($3);
		$$ = $1;
	}
			|
	{
		$$ = new ArrayAST();
	}
			;
ConstDef	:	IDENT ConstArr '=' ConstInitVal
	{
		std::vector<int> tmp = ((PointerOfArrarAST)$2)->Getrawarraytype();
		tmp = Arraydimprocess(tmp);
		std::map<int, PointerOfBaseAST> t = ((PointerOfArrayTree)$4)->Flatten(tmp); // Move constructor
		DefVarAST *p = new DefVarAST;
		PointerOfBaseSymtab newsym = NewSymtab(1,true, $1, p);
		Insertsymentry(newsym);
		((VarSymtab *)newsym)->Setarraytype(tmp);
		p->sym = newsym;
		p->init = t;
		$$ = p;
	 	std::string tmp2 = ("T");
        tmp2 = tmp2 + std::to_string(numofend);
        p->addr = tmp2;
        numofend++;
	}
			;
ConstInitValArr	:	ConstInitValArr ',' ConstInitVal
	{
		/* ConstInitValArr: ArrayTree */
		((PointerOfArrayTree)$1)->Insertsublist($3);
		/* Maintain positioning info */

		$$ = $1;
	}
				|	ConstInitVal
	{
		/* ConstInitValArr: ArrayTree */
		PointerOfArrayTree tmp = new ArrayTree;
		tmp->Insertsublist($1);
		$$ = tmp;
	}
				;
ConstInitVal	:	ConstExp
	{
		/* ConstInitVal: ArrayTree */
		$$ = new ArrayTree($1);
	}
				|	'{' '}'
	{
		/* ConstInitVal: ArrayTree */
		$$ = new ArrayTree($1, $2);

	}
				|	'{' ConstInitValArr '}'
	{
		/* ConstInitVal: ArrayTree */
		$$ = $2;
		
	}
				;
	VarDef		:	IDENT ConstArr
	{
		DefVarAST *p = new DefVarAST;
		std::vector<int> arrtype = ((PointerOfArrarAST)$2)->Getrawarraytype();
		arrtype = Arraydimprocess(arrtype);
		PointerOfBaseSymtab newsym = NewSymtab(1,false, $1, p);
		Insertsymentry(newsym);
		((VarSymtab*)newsym)->Setarraytype(arrtype);
		p->sym = newsym;
		$$ = p;
		std::string tmp2 = ("T");
        tmp2 = tmp2 + std::to_string(numofend);
        p->addr = tmp2;
        numofend++;
	}
			|	IDENT ConstArr '=' InitVal
	{
		std::vector<int> arrtype = ((PointerOfArrarAST)$2)->Getrawarraytype();
		arrtype = Arraydimprocess(arrtype);
		std::map<int, PointerOfBaseAST> t = ((PointerOfArrayTree)$4)->Flatten(arrtype);
		DefVarAST *p = new DefVarAST;
		PointerOfBaseSymtab newsym = NewSymtab(1,false, $1, p);
		Insertsymentry(newsym);
		((VarSymtab *)newsym)->Setarraytype(std::move(arrtype));
		p->sym = newsym;
		p->init = std::move(t);
		$$ = p;
		std::string tmp2 = ("T");
        tmp2 = tmp2 + std::to_string(numofend);
        p->addr = tmp2;
        numofend++;
	}
		;
VarDefArr	:	VarDefArr ',' VarDef
	{
		PointerOfArrarAST p = (PointerOfArrarAST)$1;
		p->Insertastlist($3);
		$$ = p;
	}
			|	VarDef
	{
		PointerOfArrarAST p = new ArrayAST;
		p->Insertastlist($1);
		$$ = p;	
	}
			;
VarDecl		:	INT VarDefArr ';'
	{
		$$ = $2;
	}
			;

			
InitValArr	:	InitValArr ',' InitVal
	{
		((PointerOfArrayTree)$1)->Insertsublist($3);
		$$ = $1;
	}
			|	InitVal
	{
		PointerOfArrayTree p = new ArrayTree;
		p->Insertsublist($1); 
		$$ = p;
	}
			;
InitVal		:	Exp
	{
		$$ = new ArrayTree($1);
	}
			|	'{' '}'
	{
		$$ = new ArrayTree($1, $2);

	}
			|	'{' InitValArr '}'
	{
		$$ = $2;
	}
			;

FuncFParams	:	FuncFParams ',' FuncFParam
	{
		((PointerOfArrarAST)$1)->Insertastlist($3);
		$$ = $1;
	}
			|	FuncFParam
	{
		PointerOfArrarAST p = new ArrayAST;
		p->Insertastlist($1);
		$$ = p;
	}
			;
FuncFParam	:	INT IDENT
	{
		DefVarAST *p = new DefVarAST;
		PointerOfBaseSymtab newsym = NewSymtab(1,false, $2, p);
		Insertsymentry(newsym);
		std::vector<int> tmp (1,1);
		((VarSymtab *)newsym)->Setarraytype(tmp);
		p->sym = newsym;
		$$ = p;
		std::string tmp2 = ("p");
        tmp2 = tmp2 + std::to_string(numofdec);
        p->addr = tmp2;
        numofdec++;
	}
			|	INT IDENT '[' ']' ConstArr
	{
		DefVarAST *p = new DefVarAST;
		PointerOfBaseSymtab newsym = NewSymtab(1,false, $2, p);
		Insertsymentry(newsym);
		std::vector<int> arrtype = ((PointerOfArrarAST)$5)->Getrawarraytype();
		arrtype = Arraydimprocess(arrtype);
		arrtype.push_back(0);
		((VarSymtab *)newsym)->Setarraytype(std::move(arrtype));
		p->sym = newsym;
		$$ = p;
		std::string tmp2 = ("p");
        tmp2 = tmp2 + std::to_string(numofdec);
        p->addr = tmp2;
        numofdec++;
	}
			;
FuncDef		:	FuncDefproto '{' BlockItemArr '}'
	{
		DefFunAST *p = (DefFunAST*)$1;
		p->body = $3;
		$$ = $1;
		if(*(p->sym->strptr) == "main")
			mainptr = p;
		Popsymtab();
		numofdec = 0;
	}
			;
FuncDefproto	:	FuncDefHead ')'
	{
		$$ = $1;
	}
				|	FuncDefHead FuncFParams ')'
	{
		DefFunAST *p = (DefFunAST *)$1;
		p->fparams = ((PointerOfArrarAST)$2)->astlist; 
		for(auto i: p->fparams)
			((FuncSymtab *)(p->sym))->fparams.push_back(((DefVarAST *)i)->sym);
		$$ = $1;
	}
				;
FuncDefHead	:	INT IDENT '('
	{
		DefFunAST *p = new DefFunAST;
		PointerOfBaseSymtab newsym = NewSymtab(0,false, $2, p);
		Insertsymentry(newsym);
		p->sym = newsym;
		$$ = p;
		Pushsymtab();
		curfundef = p;
		numofdec = 0;
		std::string tmp2 = ("f_");
		tmp2 = tmp2 + *(p->sym->strptr);
		p->addr = tmp2;
	}
			|	VOID IDENT '('
	{
		DefFunAST *p = new DefFunAST;
		PointerOfBaseSymtab newsym = NewSymtab(0,true, $2, p);
		Insertsymentry(newsym);
		p->sym = newsym;
		$$ = p;
		Pushsymtab();
		curfundef = p;
		numofdec = 0;
		std::string tmp2 = ("f_");
		tmp2 = tmp2 + *(p->sym->strptr);
		p->addr = tmp2;
	}
			;

BlockItemArr	:	BlockItemArr DefVar
	{
		FunStmtAST *p = (FunStmtAST *)$1;
		p->Insertclause(((ArrayAST*)$2)->astlist); //
		$$ = $1;
	}
				|	BlockItemArr Stmt
	{
		FunStmtAST *p = (FunStmtAST *)$1;
		p->Insertclause($2);
		$$ = $1;
	}
				|
	{
		$$ = new FunStmtAST;
	}
				;
FunStatement		:	'{' BlockStart BlockItemArr '}'
	{
		Popsymtab();
		$$ = $3;
	}
			;
BlockStart	:
	{
		Pushsymtab();
		$$ = nullptr;
	}
			;
Stmt		:	';'
	{
		$$ = nullptr;
	}
			|	LVal '=' Exp ';'
	{
		VoluAST *p = new VoluAST;
		LeftValueAST *q = (LeftValueAST *)$1;
		p->lval = $1;
		p->exp = $3;
		$$ = p;
	}
			|	Exp ';'
	{
		$$ = $1;
	}
			|	FunStatement
	{
		$$ = $1;
	}
			|	IF '(' Cond ')' Stmt
	{
		IfAST *p = new IfAST;
		p->cond = $3;
		p->then = $5;
		$$ = p;
	}
			|	IF '(' Cond ')' Stmt ELSE Stmt
	{
		IfAST *p = new IfAST;
		p->cond = $3;
		p->then = $5;
		p->els = $7;
		$$ = p;
	}
			|	WhileHead '(' Cond ')' Stmt
	{
		WhileAST *p = (WhileAST *)$1;
		p->cond = $3;
		p->body = $5;
		$$ = p;
		whilestk.pop_back();
	}
			|	BREAK ';'
	{
		if(whilestk.empty())
			$$ = nullptr;
		else
		{
			BreakAST *p = new BreakAST;
			$$ = p;
		}

	}
			|	CONTINUE ';'
	{
		if(whilestk.empty())
		{
			$$ = nullptr;
		}
		else
		{
			ContinueAST *p = new ContinueAST;
			$$ = p;
		}
	}
			|	RETURN ';'
	{
		RetAST *p = new RetAST;
		FuncSymtab *q = (FuncSymtab *)((DefFunAST *)p->back)->sym;
		if(!q->isvoid)
		{
			Token* r = new Token(0, INT_CONST);
			BinaryAST *s = new BinaryAST(r);
			delete r;
			p->exp = s;
		}
		$$ = p;

	}
			|	RETURN Exp ';'
	{
		RetAST *p = new RetAST;
		FuncSymtab *q = (FuncSymtab *)((DefFunAST *)p->back)->sym;
		if(!q->isvoid){
			p->exp = $2;
			$$ = p;	
		}
		$$ = p;

	}
			;
WhileHead	:	WHILE
	{
		WhileAST *p = new WhileAST;
		$$ = p;
		whilestk.push_back(p);
	}
			;

Exp			:	AddExp
	{
		$1->Eval();
		$$ = $1;
	}
			;
Cond		:	LOrExp
	{
		$1->Eval();
		$$ = $1;
	}
			;
ExpArr		:	ExpArr '[' Exp ']'
	{

		((PointerOfArrarAST)$1)->Insertastlist($3);
		$$ = $1;
	}
			|
	{
		$$ = new ArrayAST;
	}
			;
LVal		:	IDENT ExpArr
	{
		$$ = new LeftValueAST($1, $2);
	}
			;
PrimaryExp	:	'(' Exp ')'
	{
		$$ = $2;
	}
			|	LVal
			|	INT_CONST
	{
		$$ = new BinaryAST($1);
	}
			;
FuncRParams	:	FuncRParams ',' Exp
	{
		((ArrayAST *)$1)->Insertastlist($3);	
	}
			|	Exp
	{
		ArrayAST *p = new ArrayAST;
		p->Insertastlist($1);
		$$ = p;
	}
			;
UnaryExp	:	PrimaryExp
			|	IDENT '(' ')'
	{
		CallFunAST *ptr = new CallFunAST($1);
		$$ = ptr;
	}
			|	IDENT '(' FuncRParams ')'
	{
		CallFunAST *ptr = new CallFunAST($1);
		ptr->rparams = ((PointerOfArrarAST)$3)->astlist; 
		$$ = ptr;
	}
			|	UnaryOp UnaryExp
	{
		if($2->type == ASTType::SingExpr) {
			((SingleAST *)$2)->Coalesce($1);
			$$ = $2;
		}
		else $$ = new SingleAST($2, $1);
	}
			;
UnaryOp		:	'+'
			|	'-'
			|	'!'
			;
MulExp		:	UnaryExp
			|	MulExp '*' UnaryExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}

	|	MulExp '/' UnaryExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}

	|	MulExp '%' UnaryExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}

	
			;
AddExp		:	MulExp
			|	AddExp '+' MulExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			|	AddExp '-' MulExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			;
RelExp		:	AddExp
			|	RelExp '<' AddExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			|	RelExp '>' AddExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			|	RelExp LE AddExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			|	RelExp GE AddExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			;
EqExp		:	RelExp
			|	EqExp EQ RelExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			|	EqExp NE RelExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			;
LAndExp		:	EqExp
			|	LAndExp AND EqExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			;
LOrExp		:	LAndExp
			|	LOrExp OR LAndExp
	{
		$$ = new BinaryAST($1, $3, $2);
	}
			;
ConstExp	:	AddExp
	{
		$1->Eval();
		$$ = $1;
	}
			;

%%
