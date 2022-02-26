%{
#include <string.h>
#include <stdio.h>
#include "util.h"
int yyerror(const char []){};
extern int yylex(void);
TreeNode *SyntaxTree;
%}

%token VAR LABEL IF GOTO STORE LOAD LOADADDR MALLOC
%token FUN END CALL RETURN INT
%token AND OR EQ NEQ

%% 
root		: program
	  			{ SyntaxTree = $1; }

program 	: stmt program
		 		{ 
				  $$ = NewNode(Stmt_K,Seq_K);
				  $$->child[0] = $1;
				  $$->child[1] = $2;
				  $1->global = true;
				}
			| func program
		 		{
				  $$ = NewNode(Stmt_K,Seq_K);
				  $$->child[0] = $1;
				  $$->child[1] = $2;
				}
			| func
		 		{ 
				  $$ = NewNode(Stmt_K,Seq_K);
				  $$->child[0] = $1;
				}
			;

func 		: FUN '[' INT ']' '[' INT ']' stmt_seq END FUN
		 		{ 
				  $$ = $1;
				  $$->child[0] = $3;
				  $$->child[1] = $6;
				  $$->child[2] = $8;
				}
			;

stmt_seq 	: stmt_seq stmt
		 		{
				  	$$ = NewNode(Stmt_K,Seq_K);
				  	$$->child[0] = $1;
					$$->child[1] = $2;
				}
		  	|  { $$ = NULL; }
			;

stmt 		: LABEL ':'
		 		{ $$ = NewNode(Stmt_K,Label_K);
				  $$->child[0] = $1;
				}
			| GOTO LABEL
		 		{ $$ = NewNode(Stmt_K,Goto_K);
				  $$->child[0] = $2;
				}
			| IF LogicExp GOTO LABEL
		 		{ $$ = NewNode(Stmt_K,If_K);
				  $$->child[0] = $2;
				  $$->child[1] = $4;
				}
			| VAR '=' exp
		 		{ $$ = NewNode(Stmt_K,Assign_K);
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| VAR '=' MALLOC INT
				{ $$ = NewNode(Stmt_K,Def_K);
				  $$->child[0] = $1;
				  $$->child[1] = $4;
				}
			| VAR '[' val ']' '=' val
		 		{ $$ = NewNode(Stmt_K,Assign_K);
				  $$->child[0] = NewNode(Exp_K,Array_K);
				  $$->child[0]->child[0] = $1;
				  $$->child[0]->child[1] = $3;
				  $$->child[1] = $6;
				}
			| VAR '=' VAR '[' val ']'
		 		{ $$ = NewNode(Stmt_K,Assign_K);
				  $$->child[0] = $1;
				  $$->child[1] = NewNode(Exp_K,Array_K);
				  $$->child[1]->child[0] = $3;
				  $$->child[1]->child[1] = $5;
				}
			| CALL FUN
		 		{ $$ = NewNode(Stmt_K,OFunc_K);
				  $$->child[0] = $2;
				}
			| RETURN
		 		{ $$ = NewNode(Stmt_K,Return_K); }
			| STORE VAR INT
				{ $$ = NewNode(Stmt_K,Store_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOAD INT VAR
				{ $$ = NewNode(Stmt_K,Load_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOADADDR INT VAR
				{ $$ = NewNode(Stmt_K,Loadaddr_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOAD VAR VAR
				{ $$ = NewNode(Stmt_K,Load_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOADADDR VAR VAR
				{ $$ = NewNode(Stmt_K,Loadaddr_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			;

exp			: val '+' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Plus;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '-' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Minus;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '*' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Times;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '/' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Over;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '%' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Mod;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| LogicExp
			| '-' val
		 		{ $$ = NewNode(Exp_K,SOp_K);
				  $$->attr.op = Minus;
				  $$->child[0] = $2;
				}
			| val
				{ $$ = $1; }
			;

LogicExp 	: val AND val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = And;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
		  	| val OR val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Or;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '>' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Gt;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '<' val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Lt;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val EQ val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Eq;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val NEQ val
		 		{ $$ = NewNode(Exp_K,DOp_K);
				  $$->attr.op = Neq;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| '!' val
		 		{ $$ = NewNode(Exp_K,SOp_K);
				  $$->attr.op = Not;
				  $$->child[0] = $2;
				}
			;

val 		: VAR
				{ $$ = $1; }
	  		| INT
				{ $$ = $1; }
			;

%% 
