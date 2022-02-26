#ifndef _UTIL_H_
#define _UTIL_H_

#include <iostream>
class Var {
public:
	char kind;
	int index;

	Var(char kind, int index)
    {
		std::cout << "草" << std::endl;
        this->kind = kind;
	    this->index = index;
    }
	Var(){;}

	friend bool operator<(Var v1, Var v2);
};


typedef enum 
{   
    Func_K, Stmt_K, Exp_K, If_K, Assign_K, Def_K, Return_K, 
    Seq_K, OFunc_K, Label_K, Goto_K, Param_K, Load_K, Loadaddr_K, Store_K,
    SOp_K, DOp_K, Const_K, Var_K, Array_K, CFunc_K,
    Plus, Minus, Times, Over, Mod, Lt, Gt, Eq, Neq, Not, And, Or,
    Reserve_Type
}Type;
#define CHILDREN_NUM 4

class TreeNode
{
public:
	TreeNode *child[CHILDREN_NUM];
	Type nk;
	union {Type stmt; Type exp;} kind;
	union {Type op; int val; Var Var_info; char *func_name;} attr;
	TreeNode *parentnode;
	bool global;
};


#define YYSTYPE TreeNode *
extern YYSTYPE SyntaxTree;
extern FILE *Tigger_code;
TreeNode * NewNode(Type mode, Type kind);
void PrintRiscV(TreeNode *t);

#endif
