#pragma once
#ifndef AST_EEYORE_HPP
#define AST_EEYORE_HPP

#include <string>
#include <list>
#include <vector>
#include <cstring>
#include <unordered_map>
using namespace std;

#ifndef Debug
#define Debug 1
#endif

//全局字符对应表
/*
例如 Eeyore里，var T0, 有初始化 T0 = 1
需翻译为tigger后是 v0 = 1
则插入表项 ("T0",{"v0", 1})
*/
static int vn = 0;
class Table{
    public:
        string tiggerName;
        int val;
        bool isArray;
        int arrNum;
        Table(string name, bool isArray_, int arraynum_):tiggerName(name), val(0),isArray(isArray_),arrNum(arraynum_){};
        Table(string name, bool isArray_):tiggerName(name),isArray(isArray_),val(0){};
        Table(string name):tiggerName(name),val(0),isArray(false){};
};
static unordered_map<string, Table> globalVar;
typedef unordered_map<string, Table>::iterator EntryIT;
static int nowParam = 0;
bool IsGlobalVar(string s);


static list<string> tiggerCode;
int convert2int(const string s);
string convert2string(int num);

typedef enum{
    Plus, Minus, Multi, Divi, Mod, Not, More, Less, MorEq, LorEq, \
    And, Or, Neq, Eq
}opType;

typedef enum{
    symbol, functiontype, num, op, kw, label
}tokenType;

const string tokenType2string[] = {"symbol", "function", "num", "op", "keyword", "label"};

class Token{
    public:
        int type;
        string tokenString;
        bool isTemp; //t[0-9]+ is ture. T[0-9]+ is false.
        Token(int type_):type(type_){isTemp = false;}
        Token(int type_, const char* s):type(type_),tokenString(s){isTemp = false; }
        Token(int type_, const char* s, bool b):type(type_),tokenString(s),isTemp(b){}
};

class BaseEeyoreAST;
class ProgramAST;
class DefVarAST; class InitAST; class DefFunAST;
class FunHeadAST; class VoluAST; class FunEndAST;
//exp:
class BinaryAST; class SingleAST; class AssignAST;
class IfAST; class GotoAST; class CallAST; class RetAST;
class LabelAST; class ParamAST;
class RightValueAST;
typedef enum{
    BaseEeyore, Program, Decl, Init, FunDef, FunHead,
    Stmts, FunEnd, Exp
}ASTtype;

class BaseEeyoreAST{
    public:
        ASTtype type;
        DefFunAST* funcName; //指向该函数的FunDefAST
        virtual void generator(){};
        BaseEeyoreAST():type(BaseEeyore){};
};
class ProgramAST:public BaseEeyoreAST{
    public:
        vector<BaseEeyoreAST*> globalDecl;
        vector<BaseEeyoreAST*> globalInit;
        vector<BaseEeyoreAST*> funDefs;
        ProgramAST(){;}
        ProgramAST(BaseEeyoreAST* p,BaseEeyoreAST* decl,BaseEeyoreAST* initi,BaseEeyoreAST* fund)
        {
            if(p!= nullptr) {
                ProgramAST* pp=(ProgramAST*) p;
                if(decl != nullptr)
                    pp -> globalDecl.push_back(decl);
                if(initi != nullptr)
                    pp -> globalInit.push_back(initi);
                if(fund != nullptr)
                    pp -> funDefs.push_back(fund);
            }
        }
        virtual void generator();
        ~ProgramAST(){
            for(auto i:globalDecl) if(i) delete i;
            for(auto i:globalInit) if(i) delete i;
            for(auto i:funDefs) if(i) delete i;
        };
};
class DefVarAST:public BaseEeyoreAST{
    public:
        bool isArray;
        bool isTemp; //t[0-9]+ is ture. T[0-9]+ is false.
        string varName;
        int num;
        virtual void generator();
        DefVarAST(string name, string num_, bool isTemp_):varName(name), num(convert2int(num_)), \
        isArray(true), isTemp(isTemp_){ type=Decl; }
        DefVarAST(string name, bool isTemp_):varName(name), \
        isArray(false), isTemp(isTemp_){ type=Decl; }
        ~DefVarAST(){};
};
class InitAST:public BaseEeyoreAST{
    public:
        string leftName;
        int leftNum;
        int rightNum;
        bool isArray;
        virtual void generator();
        InitAST(string name, int right):leftName(name),rightNum(right), \
        isArray(false){ type=Init; } //非数组
        InitAST(string name, int left, int right):leftName(name),leftNum(left),rightNum(right), \
        isArray(true){ type=Init; } //数组
        ~InitAST(){};
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
        virtual void generator();
        DefFunAST(BaseEeyoreAST* p1, BaseEeyoreAST* p2, BaseEeyoreAST* p3): \
        head(p1), StmtsASTptr(p2), end(p3){ type=FunDef; };
        ~DefFunAST(){
            if(head) delete head;
            if(StmtsASTptr) delete StmtsASTptr;
            if(end) delete end;
        }
};
class FunHeadAST:public BaseEeyoreAST{
    public:
        string funName;
        int paraNum;
        virtual void generator();
        FunHeadAST(string name, int a):funName(name), \
        paraNum(a){ type=FunHead; }
        ~FunHeadAST(){};
};
class VoluAST:public BaseEeyoreAST{
    public:
        vector<BaseEeyoreAST*> stmts;
        virtual void generator();
        VoluAST(){type=Stmts;}
        ~VoluAST(){
            for(auto i:stmts){ if(i)delete i; }
        }
};
class FunEndAST:public BaseEeyoreAST{
    public:
        string funName;
        virtual void generator();
        FunEndAST(string s):funName(s){};
};

//exp:
//todo
class BinaryAST:public BaseEeyoreAST{
    public:
        string leftName;
        RightValueAST* firstVal; RightValueAST* secVal;
        string op;
        virtual void generator();
        BinaryAST(string s1, RightValueAST* s2, string op_, RightValueAST* s3): \
        leftName(s1), op(op_){type=Exp;firstVal=s2;secVal=s3;}
        ~BinaryAST(){};
};
//todo
class SingleAST:public BaseEeyoreAST{
    public:
        string leftName;
        RightValueAST* firstVal;
        string op;
        virtual void generator();
        SingleAST(string s1, string op_, RightValueAST* s2): \
        leftName(s1), op(op_){type=Exp;firstVal=s2;};
};

//todo
class AssignAST:public BaseEeyoreAST{
    public:
        /*
        AssignType = 1: SYMBOL "=" RightValue
        AssignType = 2: SYMBOL "[" RightValue "]" "=" RightValue
        AssignType = 3: SYMBOL "=" SYMBOL "[" RightValue "]"
        */
        int AssignType;
        string val1;
        RightValueAST* val2;
        string s2;
        RightValueAST* val3;
        virtual void generator();
        //1
        AssignAST(string s1, RightValueAST* s2, int n):val1(s1),AssignType(n){type=Exp;val2=s2;val3=nullptr;}
        //2
        AssignAST(string s1, RightValueAST* s2, RightValueAST* s3, int n):val1(s1),AssignType(n){type=Exp;val2=s2;val3=s3;}
        //3
        AssignAST(string s1, string s2_, RightValueAST* s3, int n):val1(s1),AssignType(n) \
        {type=Exp;val3=s3;s2=s2_;}
        ~AssignAST(){};
};

//todo
class IfAST:public BaseEeyoreAST{
    public:
        //"if" RightValue OP RightValue "goto" LABEL
        RightValueAST* val1;
        RightValueAST* val2;
        string label;
        string op;
        IfAST(RightValueAST* s1, string op_, RightValueAST* s2, string label_): \
        op(op_), label(label_){type=Exp;val1=s1;val2=s2;}
        ~IfAST(){};
        virtual void generator();
};
class GotoAST:public BaseEeyoreAST{
    public:
        //"goto" LABEL
        string label;
        GotoAST(string s):label(s){type=Exp;};
        ~GotoAST(){};
        virtual void generator();
};
class CallAST:public BaseEeyoreAST{
    public:
        //"call" FUNCTION
        //SYMBOL "=" "call" FUNCTION
        string funName;
        bool isAssign;
        string leftName;
        CallAST(string s):funName(s),isAssign(false){type=Exp;};
        CallAST(string s1, string s2):leftName(s1),funName(s2),isAssign(true){type=Exp;};
        ~CallAST(){};
        virtual void generator();
};

//todo
class RetAST:public BaseEeyoreAST{
    public:
        bool isVoid;
        RightValueAST* retVal;
        RetAST(RightValueAST* s, bool isVoid_):isVoid(isVoid_){type=Exp;retVal=s;};
        ~RetAST(){};
        virtual void generator();
};
class LabelAST:public BaseEeyoreAST{
    public:
        //LABEL ":"
        string Label;
        LabelAST(string s):Label(s){type=Exp;};
        virtual void generator();
};

//todo
class ParamAST:public BaseEeyoreAST{
    public:
        //"param" RightValue
        RightValueAST* p;
        ParamAST(RightValueAST* ss){type=Exp;p=ss;};
        virtual void generator();
};

class RightValueAST:public BaseEeyoreAST{
    public:
        // SYMBOL | NUM;
        string s;
        bool isNum;
        RightValueAST(string ss, bool isNum_):s(ss),isNum(isNum_){type=Exp;}
        virtual void generator();
};
#endif
