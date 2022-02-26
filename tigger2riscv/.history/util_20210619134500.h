#ifndef _UTIL_H_
#define _UTIL_H_

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif

#define MAXRESERVED 8

#define MAX_TOKEN_LEN 80

extern int lineno;

class Var {
public:
	char kind;
	int index;
	Var(char kind, int index);
	Var();
	friend bool operator<(Var v1, Var v2);
};



typedef enum {Func_K, Stmt_K, Exp_K} Type;
typedef enum {If_K, Assign_K, Def_K, Return_K, Seq_K, OFunc_K, Label_K, Goto_K, Param_K, Load_K, Loadaddr_K, Store_K} Type;
typedef enum {SOp_K, DOp_K, Const_K, Var_K, Array_K, CFunc_K} Type;
typedef enum {Plus, Minus, Times, Over, Mod, Lt, Gt, Eq, Neq, Not, And, Or} Type;

#define CHILDREN_NUM 4

class TreeNode
{
public:
	TreeNode *child[CHILDREN_NUM];
	int lineno;
	Type nk;
	union {Type stmt; Type exp;} kind;
	union {Type op; int val; Var Var_info; char *func_name;} attr;
	TreeNode *parentnode;
	bool global;
};

extern TreeNode *SyntaxTree;


extern FILE *Tigger_code;

TreeNode *newFuncNode();

TreeNode *NewNode(Type);

TreeNode *NewNode(Type);

char *copyString(char *);

void PrintRiscV(TreeNode *t);

void convert(TreeNode *t);

#endif
