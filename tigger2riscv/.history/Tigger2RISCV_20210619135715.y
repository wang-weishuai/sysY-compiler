%{
#include <string.h>
#include <stdio.h>
#include "globals.h"
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
		 		{ $$ = NewNode(Seq_K);
				  $$->child[0] = $1;
				  $$->child[1] = $2;
				  $1->global = true;
				}
			| func program
		 		{ $$ = NewNode(Seq_K);
				  $$->child[0] = $1;
				  $$->child[1] = $2;
				}
			| func
		 		{ $$ = NewNode(Seq_K);
				  $$->child[0] = $1;
				}
			;

func 		: FUN '[' INT ']' '[' INT ']' stmt_seq END FUN
		 		{ $$ = $1;
				  $$->child[0] = $3;
				  $$->child[1] = $6;
				  $$->child[2] = $8;
				}
			;

stmt_seq 	: stmt_seq stmt
		 		{
				  	$$ = NewNode(Seq_K);
				  	$$->child[0] = $1;
					$$->child[1] = $2;
				}
		  	|  { $$ = NULL; }
			;

stmt 		: LABEL ':'
		 		{ $$ = NewNode(Label_K);
				  $$->child[0] = $1;
				}
			| GOTO LABEL
		 		{ $$ = NewNode(Goto_K);
				  $$->child[0] = $2;
				}
			| IF LogicExp GOTO LABEL
		 		{ $$ = NewNode(If_K);
				  $$->child[0] = $2;
				  $$->child[1] = $4;
				}
			| VAR '=' exp
		 		{ $$ = NewNode(Assign_K);
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| VAR '=' MALLOC INT
				{ $$ = NewNode(Def_K);
				  $$->child[0] = $1;
				  $$->child[1] = $4;
				}
			| VAR '[' val ']' '=' val
		 		{ $$ = NewNode(Assign_K);
				  $$->child[0] = NewNode(Array_K);
				  $$->child[0]->child[0] = $1;
				  $$->child[0]->child[1] = $3;
				  $$->child[1] = $6;
				}
			| VAR '=' VAR '[' val ']'
		 		{ $$ = NewNode(Assign_K);
				  $$->child[0] = $1;
				  $$->child[1] = NewNode(Array_K);
				  $$->child[1]->child[0] = $3;
				  $$->child[1]->child[1] = $5;
				}
			| CALL FUN
		 		{ $$ = NewNode(OFunc_K);
				  $$->child[0] = $2;
				}
			| RETURN
		 		{ $$ = NewNode(Return_K); }
			| STORE VAR INT
				{ $$ = NewNode(Store_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOAD INT VAR
				{ $$ = NewNode(Load_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOADADDR INT VAR
				{ $$ = NewNode(Loadaddr_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOAD VAR VAR
				{ $$ = NewNode(Load_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			| LOADADDR VAR VAR
				{ $$ = NewNode(Loadaddr_K);
				  $$->child[0] = $2;
				  $$->child[1] = $3;
				}
			;

exp			: val '+' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Plus;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '-' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Minus;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '*' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Times;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '/' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Over;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '%' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Mod;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| LogicExp
			| '-' val
		 		{ $$ = NewNode(SOp_K);
				  $$->attr.op = Minus;
				  $$->child[0] = $2;
				}
			| val
				{ $$ = $1; }
			;

LogicExp 	: val AND val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = And;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
		  	| val OR val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Or;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '>' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Gt;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val '<' val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Lt;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val EQ val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Eq;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| val NEQ val
		 		{ $$ = NewNode(DOp_K);
				  $$->attr.op = Neq;
				  $$->child[0] = $1;
				  $$->child[1] = $3;
				}
			| '!' val
		 		{ $$ = NewNode(SOp_K);
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

int yyerror(const char message[])
{
	return 0;
}
