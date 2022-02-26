#ifndef _TIGGERBASIC_
#define _TIGGERBASIC_

#include <string>
#include <list>
#include <vector>
#include <cstring>
#include <unordered_map>
using namespace std;
static int vn = 0;
class Table;
class Token;
static unordered_map<string, Table> globalVar;
typedef unordered_map<string, Table>::iterator EntryIT;
static int nowParam = 0;
bool IsGlobalVar(string s);
static list<string> tiggerCode;
int convert2int(const string s);
string convert2string(int num);
enum{symbol, functiontype, num, op, kw, label};



typedef enum{
    BaseEeyore, Program, Decl, Init, FunDef, FunHead, Stmts, FunEnd, Exp
}ASTtype;
class Table
{
    public:
        string tiggerName;
        int val;
        bool isArray;
        int arrNum;
        Table(string name, bool isArray_, int arraynum_):tiggerName(name), val(0),isArray(isArray_),arrNum(arraynum_){};
};

class Token{
    public:
        int type;
        string tokenString;
        bool isTemp;
        Token(int type_, const char* s, bool b):type(type_),tokenString(s),isTemp(b){}
};
class BaseEeyoreAST{
    public:
        ASTtype type;
        DefFunAST* funcName;
        virtual void generator(){};
        BaseEeyoreAST():type(BaseEeyore){};
};


class BinaryAST:public BaseEeyoreAST{
    public:
        string leftName;
        RightValueAST* firstVal; RightValueAST* secVal;
        string op;
        virtual void generator() override;
        BinaryAST(string s1, RightValueAST* s2, string op_, RightValueAST* s3): leftName(s1), op(op_){type=Exp;firstVal=s2;secVal=s3;}
};

class SingleAST:public BaseEeyoreAST{
    public:
        string leftName;
        RightValueAST* firstVal;
        string op;
        virtual void generator() override;
        SingleAST(string s1, string op_, RightValueAST* s2): leftName(s1), op(op_){type=Exp;firstVal=s2;};
};


class AssignAST:public BaseEeyoreAST{
    public:
        int AssignType;
        string val1;
        RightValueAST* val2;
        string s2;
        RightValueAST* val3;
        virtual void generator() override;
        AssignAST(string s1_, string s2_, RightValueAST* p1,  RightValueAST* p2, int n)
        {
            val1 = s1_;
            s2 = s2_;
            val2 = p1;
            val3 = p2;
            AssignType = n;
        }
};

class IfAST:public BaseEeyoreAST{
    public:
        RightValueAST* val1;
        RightValueAST* val2;
        string label;
        string op;
        IfAST(RightValueAST* s1, string op_, RightValueAST* s2, string label_): op(op_), label(label_){type=Exp;val1=s1;val2=s2;}
        virtual void generator() override;
};
class GotoAST:public BaseEeyoreAST{
    public:
        string label;
        GotoAST(string s,ASTtype t):label(s){type = t;}
        virtual void generator() override;
};
class CallAST:public BaseEeyoreAST{
    public:
        bool isAssign;
        string funName;
        string leftName;
        CallAST(string s):funName(s),isAssign(false){type=Exp;};
        CallAST(string s1, string s2, bool b, ASTtype t):leftName(s1),funName(s2),isAssign(b){type=t;};
        virtual void generator() override;
};

class RetAST:public BaseEeyoreAST{
    public:
        bool isVoid;
        RightValueAST* retVal;
        RetAST(RightValueAST* s, bool isVoid_):isVoid(isVoid_){type=Exp;retVal=s;};
        virtual void generator() override;
};
class LabelAST:public BaseEeyoreAST{
    public:
        string Label;
        LabelAST(string s):Label(s){type=Exp;};
        virtual void generator() override;
};

class ParamAST:public BaseEeyoreAST{
    public:
        RightValueAST* p;
        ParamAST(RightValueAST* ss){type=Exp;p=ss;};
        virtual void generator() override;
};

class RightValueAST:public BaseEeyoreAST{
    public:
        string s;
        bool isNum;
        RightValueAST(string ss, bool isNum_):s(ss),isNum(isNum_){type=Exp;}
};
class ProgramAST:public BaseEeyoreAST{
    public:
        vector<BaseEeyoreAST*> globalDecl;
        vector<BaseEeyoreAST*> globalInit;
        vector<BaseEeyoreAST*> funDefs;
        ProgramAST(){;}
        ProgramAST(BaseEeyoreAST* p,BaseEeyoreAST* decl,BaseEeyoreAST* initi,BaseEeyoreAST* fund)
        {    
            ProgramAST* pp = new ProgramAST;            
            if(p != NULL)        
                pp = (ProgramAST*) p;
            if(decl != NULL)
                pp -> globalDecl.push_back(decl);
            if(initi != NULL)
                pp -> globalInit.push_back(initi);
            if(fund != NULL)
                pp -> funDefs.push_back(fund);
        }
        virtual void generator() override;
};
class DefVarAST:public BaseEeyoreAST{
    public:
        bool isArray;
        bool isTemp; //t[0-9]+ is ture. T[0-9]+ is false.
        string varName;
        int num;
        virtual void generator() override;
        DefVarAST(string name, string num_, bool isArray_, bool isTemp_):varName(name), isArray(isArray_), isTemp(isTemp_)
        { 
            type=Decl; 
            if(num_.length()) num = convert2int(num_);
            else num = 0;
        }
};
class InitAST:public BaseEeyoreAST{
    public:
        string leftName;
        int leftNum;int rightNum;
        bool isArray;
        InitAST(string name, int left, int right, bool isarr):leftName(name), leftNum(left), rightNum(right), isArray(isarr){ type=Init; } //数组
};
class DefFunAST:public BaseEeyoreAST{
    public:
        BaseEeyoreAST* head;
        BaseEeyoreAST* StmtsASTptr;
        BaseEeyoreAST* end;
        int paraNum;
        int tNum;
        int TNum;
        unordered_map<string, Table> tempVar;//映射表
        void DebugPrint();
        virtual void generator() override;
        DefFunAST(BaseEeyoreAST* p1, BaseEeyoreAST* p2, BaseEeyoreAST* p3, ASTtype t): head(p1), StmtsASTptr(p2), end(p3){ type=t; };
};
class FunHeadAST:public BaseEeyoreAST{
    public:
        string funName;
        int paraNum;
        virtual void generator() override;
        FunHeadAST(string name, int a):funName(name), paraNum(a){ type=FunHead; }
};
class VoluAST:public BaseEeyoreAST{
    public:
        vector<BaseEeyoreAST*> stmts;
        virtual void generator() override;
        VoluAST(){type=Stmts;}
};
class FunEndAST:public BaseEeyoreAST{
    public:
        string funName;
        virtual void generator() override;
        FunEndAST(string s):funName(s){};
};

#endif
