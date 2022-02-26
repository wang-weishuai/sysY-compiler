#include "eebasic.hpp"
#include "eesymtab.hpp"
#include "parser.tab.hpp"
#include "global.hpp"

PointerOfBaseAST mainptr = nullptr;
std::string tokenindex[1000];
std::list<std::string> strtab;

std::list<std::string>::iterator Inserttostrtab(const char *p)
{
	std::string tmp = p;
	strtab.push_front(tmp);
	return strtab.begin();
}

std::list<std::string>::iterator Inserttostrtab(std::string p)
{
	std::string tmp = p;
	strtab.push_front(tmp);
	return strtab.begin();
}

Token::Token(const char *p, int type_):strptr(Inserttostrtab(p))
{
    type = type_;
}
Token::Token(std::string p, int type_):strptr(Inserttostrtab(p))
{
    type = type_;
}

static void InitializeTokenindex(int type)
{
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
      char tmp[2]; 
      tmp[0] = type; 
      tmp[1] = 0;
      return tmp;
    }
    InitializeTokenindex(type);
    return tokenindex[type];
}

void BinaryAST::Eval()
{
  	if(isevaled || isconst) return;
		isevaled = true;
		lexp->Eval();
		rexp->Eval();
		int x = lexp -> Cval();
		int y = rexp -> Cval();
		if(lexp->Isconst() && rexp->Isconst()) 
		{        
      cval = x + y;
			if(op == '-') cval = x - y;
			if(op == '*') cval = x * y;
			if(op == '/') cval = x / y;
			if(op == '%') cval = x % y;
			if(op == '<') cval = x < y;
			if(op == '>') cval = x > y;
			if(op == LE) cval = x <= y;
			if(op == GE) cval = x >= y;
			if(op == EQ) cval = x == y;
			if(op == NE) cval = x != y;
			if(op == AND) cval = x && y;
			if(op == OR) cval = x || y;
			delete lexp; lexp = nullptr;
			delete rexp; rexp = nullptr;
		}
}
CallFunAST::CallFunAST(Token* p):BaseAST(ASTType::CallFunc)
{
  PointerOfBaseSymtab ptr = Lookupstring(p->strptr);
  sym = ptr;
}

CallFunAST::~CallFunAST()
{
   for(auto i: rparams) delete i;
}

bool CallFunAST::Checkparamlistsize()
{
  const int flen = ((FuncSymtab *)sym)->fparams.size();
  const int rlen = rparams.size();
  return rlen == flen;
}

bool CallFunAST::Isanumber()
{
  FuncSymtab * p = (FuncSymtab *)sym;
  return(!p->isvoid);
}




LeftValueAST::LeftValueAST(Token* p, PointerOfBaseAST q): BaseAST(ASTType::Lvalue), isconst(false), cval(0), isleft(false), isevaled(false) 
{
  PointerOfBaseSymtab ptr = Lookupstring(p->strptr);
  sym = ptr;
  indices = ((PointerOfArrarAST)q)->astlist; 
}

LeftValueAST::~LeftValueAST() 
{
  for(auto i: indices) 
    delete i;
}



void LeftValueAST::Eval()
{
    if(isevaled || isconst) return;
    isevaled = true;
    if(!((VarSymtab *)sym)->isconst) return;
    for(auto i: indices)
        if(!i->Isconst()) return;
    isconst = true;
    std::vector<int> index;
    for(auto i: indices)
      index.push_back(i->Cval());

    for(auto i: indices) delete i;
    indices.clear();

    int no = Arraylinearno(((VarSymtab *)sym)->tab_dimension, index);
    PointerOfBaseAST tmp = ((DefVarAST *)(sym->astptr))->Getinitvalue(no);
    if(tmp) cval = tmp->Cval();
    else cval = 0;
}

bool LeftValueAST::Isanumber()
{
    if(isconst) return true;
    const int len = indices.size();
    const int flen = ((VarSymtab *)sym)->tab_dimension.size();
    int flag = 0;
    if(len == flen - 1) 
    {
        for(auto i: indices) 
            if(!i->Isanumber()) 
            {
              flag = 1;
              return false;
            }
        return true;
    }
    return flag;
}



DefVarAST::~DefVarAST() 
{
    for(auto i:init) 
      delete i.second;
}

PointerOfBaseAST DefVarAST::Getinitvalue(int index) 
{
    auto k = init.find(index);
    if(k == init.end()) return nullptr;
    else return k->second;
}

void ArrayAST::Insertastlist(PointerOfBaseAST p) 
{
    astlist.push_back(p);
}

std::vector<int> ArrayAST::Getrawarraytype()
{
    std::vector<int> tmp;
    for(auto p: astlist)
        tmp.push_back(p->Cval());
    for(auto p: astlist) 
        delete p;
    astlist.clear();
    return tmp;
}

void ArrayTree::Insertsublist(PointerOfBaseAST p)
{
    sublist.push_back(p);
}





