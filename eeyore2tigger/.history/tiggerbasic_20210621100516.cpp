#include "tiggerbasic.hpp"
#include <iostream>
using namespace std;
bool IsGlobalVar(string s)
{
    return (globalVar.find(s)!=globalVar.end());
}

int convert2int(const string s)
{
    return atoi(s.c_str());
}

string convert2string(int num)
{
    string res = to_string(num);
    return res;
}


void ProgramAST::generator()
{
      for(auto i : globalDecl)
      {
        Table t("v"+convert2string(vn++), true, dynamic_cast<DefVarAST* >(i)->num);
        if(!(((DefVarAST* )i)->isArray))
        {
            t.isArray = false;
            t.arrNum = 0;
        }            
        globalVar.insert(pair<string,Table>(((DefVarAST*)i)->varName,t));

    }
    for(auto i : globalInit)
    {
        if(((InitAST* )i)->isArray) continue;

        EntryIT it = globalVar.find(((InitAST* )i)->leftName);
        it->second.val = ((InitAST* )i)->rightNum;
        
    }
    for(auto i : globalVar)
    {
        if(i.second.isArray==false)
            cout << i.second.tiggerName + " = " + convert2string(i.second.val) << endl;
        else
            cout << i.second.tiggerName + " = malloc " + convert2string(i.second.arrNum) << endl;
    }
    for(auto i : funDefs) i->generator();
    for(auto i: tiggerCode) cout << i << endl;
}
void DefVarAST::generator()
{
    if(isArray)
    {
        funcName->TNum+=num/4;
        if(isTemp)
        {
            funcName->TNum-=num/4;
            funcName->tNum+=num/4;
        }
        Table tmp("",true,num);
        funcName->tempVar.insert(pair<string,Table>(varName,tmp));
        return;
    }
    funcName->TNum++;
    if(isTemp)
    {
        funcName->TNum--;
        funcName->tNum++;
    }
    Table tmp("",false,0);
    funcName->tempVar.insert(pair<string,Table>(varName,tmp));
}

void DefFunAST::generator()
{
    for(auto i:((VoluAST*)StmtsASTptr)->stmts) if(i->type==Decl) i->generator();
    int i = 0;
    for(EntryIT it = tempVar.begin();it!=tempVar.end();++it)
    {
        it->second.tiggerName = convert2string(i); 
        i = (it->second.isArray == 0) ? (i + 1) : (i + (it->second.arrNum)/4 );
    }
    
    head->generator();
    for(auto i:((VoluAST*)StmtsASTptr)->stmts) if(i->type!=Decl) i->generator();
    end->generator();
}

void FunHeadAST::generator()
{
    cout << funName+" ["+convert2string(paraNum)+"]"+ " ["+convert2string(funcName->tNum+funcName->TNum + 8)+"]" << endl;
    if(paraNum>0)
        for(int i = 0;i < paraNum; ++i)
            funcName->tempVar.insert(pair<string,Table>("p"+convert2string(i),Table("a"+convert2string(i),false,0)));
}
void VoluAST::generator()
{
    for(auto i:stmts) i->generator();
}
void FunEndAST::generator()
{
    cout << "end "+funName << endl;
}

void BinaryAST::generator()
{
    bool flag2isNum = false;
    if(firstVal->isNum) 
        cout << "\tt5 = "+firstVal->s << endl;
    else
    {
        if(IsGlobalVar(firstVal->s))
        {
            EntryIT it = globalVar.find(firstVal->s);
            if(it->second.isArray == 0)
                cout << "\tload  "+it->second.tiggerName+"  t5" << endl;
            else
                cout << "\tloadaddr  "+it->second.tiggerName+"  t5" << endl;
        } 
        else 
        {
            EntryIT it = funcName->tempVar.find(firstVal->s);
            string memory = it->second.tiggerName;
            if(memory[0]!='a')
            {
                if(it->second.isArray==0)
                    cout << "\tload  "+memory+"  t5" << endl;
                else
                    cout << "\tloadaddr  "+memory+"  t5" << endl ;
            }  
            else    
                cout << "\tt5 = "+memory << endl;
        }
    }

    if(secVal->isNum) 
        flag2isNum = true;
    else
    {
        if(IsGlobalVar(secVal->s))
        {
            EntryIT it = globalVar.find(secVal->s);
            if(it->second.isArray==0) cout << "\tload  "+it->second.tiggerName+"  t6" << endl;
            else 
                cout << "\tloadaddr  "+it->second.tiggerName+"  t6" << endl;

        } 
        else 
        {
            EntryIT it = funcName->tempVar.find(secVal->s);
            string memory = it->second.tiggerName;
            if(memory[0]!='a'){
                if(it->second.isArray==0) 
                    cout << "\tload  "+memory+"  t6" << endl;
                else 
                    cout << "\tloadaddr  "+memory+"  t6" << endl;   
            }
            else    cout << "\tt6 = "+memory << endl;
        }
    }

    if(IsGlobalVar(leftName))
    {
        if(flag2isNum==false)   
            cout << "\tt4 = t5 "+op+" t6" << endl;
        else    
            cout << "\tt4 = t5 "+op + " " + (secVal->s) << endl;

        cout << "\tloadaddr  "+(globalVar.find(leftName))->second.tiggerName+"  t3"<< endl;
        cout << "\tt3[0] = t4"<< endl;
    }
    else
    {
        if(flag2isNum==false)   
            cout << "\tt4 = t5 "+op+" t6"<< endl;
        else    
            cout << "\tt4 = t5 "+op+" "+secVal->s<< endl;

        string memoryLeft = (funcName->tempVar.find(leftName))->second.tiggerName;
        if(memoryLeft[0]!='a') 
            cout << "\tstore t4 "+ memoryLeft<< endl;
        else 
            cout << "\t"+memoryLeft+" = t4"<< endl;
    }
    
}


void SingleAST::generator()
{
    if(firstVal->isNum) 
    {
        cout << "\tt5 = "+firstVal->s<< endl;
    }
    else
    {
        if(IsGlobalVar(firstVal->s))
        {
            EntryIT it = globalVar.find(firstVal->s);
            if(it->second.isArray==0)
                cout << "\tload  "+it->second.tiggerName+"  t5"<< endl;
            else
            {
                cout << "\tloadaddr  "+it->second.tiggerName+"  t5"<< endl;
            }
        } 
        else   
        {
            EntryIT it = funcName->tempVar.find(firstVal->s);
            string memory = it->second.tiggerName;
            if(memory[0]!='a'){
                if(it->second.isArray==0) 
                    cout << "\tload  "+memory+"  t5"<< endl;
                else 
                {
                    cout << "\tloadaddr  "+memory+"  t5"<< endl;
                }
            }
            else cout << "\tt5 = "+memory<< endl;
        }
    }
    cout << "\tt4 = "+op+" t5 " << endl;
    if(IsGlobalVar(leftName))
    {
        cout << "\tloadaddr  "+(globalVar.find(leftName))->second.tiggerName+"  t3"<< endl;
        cout << "\tt3[0] = t4"<< endl;
    }
    else
    {
        string memory = (funcName->tempVar.find(leftName))->second.tiggerName;
        if(memory[0]!='a') 
            cout << "\tstore t4 "+ memory << endl;
        else 
            cout << "\t"+memory+" = t4"<< endl;
    }
}


void AssignAST::generator()
{
    switch (AssignType)
    {
    case 1:
        if(val2->isNum)  
        {
            if(IsGlobalVar(val1))
            {
                cout << "\tt4 = "+val2->s<< endl;
                cout << "\tloadaddr  "+(globalVar.find(val1))->second.tiggerName+"  t3"<< endl;
                cout << "\tt3[0] = t4"<< endl;
            }
            else
            {
                string memory = (funcName->tempVar.find(val1))->second.tiggerName;
                cout << "\tt5 = "+val2->s << endl;
                if(memory[0]!='a') 
                    cout << "\tstore t5 "+memory<< endl;
                else 
                    cout << "\t"+memory+" = t5"<< endl;
            }
        }
        else
        {
            if(IsGlobalVar(val1))
            {
                if(IsGlobalVar(val2->s))
                {
                    EntryIT it = globalVar.find(val2->s);
                    if(it->second.isArray==0)
                        cout << "\tload  "+ it->second.tiggerName+"  t4"<< endl;
                    else 
                        cout << "\tloadaddr  "+ it->second.tiggerName+"  t4"<< endl;
                }
                else
                {
                    EntryIT it = funcName->tempVar.find(val2->s);
                    string memory = it->second.tiggerName;
                    if(memory[0]!='a')
                    {
                        if(it->second.isArray==0)
                            cout << "\tload  "+memory+"  t4"<< endl;
                        else 
                            cout << "\tloadaddr  "+memory+"  t4"<< endl;;
                    }
                    else
                        cout << "\tt4 = "+memory << endl;;
                }
                cout << "\tloadaddr  "+(globalVar.find(val1))->second.tiggerName+"  t3" << endl;
                cout << "\tt3[0] = t4" << endl;
            }
            else
            {
                if(IsGlobalVar(val2->s)){
                    EntryIT it = globalVar.find(val2->s);
                    if(it->second.isArray==0)
                        cout << "\tload  "+ it->second.tiggerName+"  t4" << endl;
                    else    
                        cout << "\tloadaddr  "+ it->second.tiggerName+"  t4" << endl;
                }
                else
                {
                    EntryIT it = (funcName->tempVar.find(val2->s));
                    string memory = it->second.tiggerName;
                    if(memory[0]!='a')
                    {
                        if(it->second.isArray==0)
                            cout << "\tload  "+memory+"  t4" << endl;
                        else
                            cout << "\tloadaddr  "+memory+"  t4" << endl;
                    }
                    else 
                        cout << "\tt4 = "+memory << endl;
                }
                string memoryLeft = (funcName->tempVar.find(val1))->second.tiggerName;
                if(memoryLeft[0]!='a') 
                    cout << "\tstore t4 "+memoryLeft << endl;
                else 
                    cout << "\t"+memoryLeft+" = t4" << endl;
            }
        }
        break;


    case 2:
  
        if(IsGlobalVar(val1))
        {
            cout << "\tloadaddr "+(globalVar.find(val1))->second.tiggerName+"  t1" << endl;;
        }
        else
        {
            string memory1 = (funcName->tempVar.find(val1))->second.tiggerName;
            if(memory1[0]!='a') 
                cout << "\tloadaddr "+memory1+" t1" << endl;
            else
                cout << "\tt1 = "+memory1 << endl;
        }
        if(IsGlobalVar(val2->s))
        {
            cout << "\tload "+(globalVar.find(val2->s))->second.tiggerName+"  t2" << endl;
        }
        else
        {
            if(val2->isNum)
            {
                cout << "\tt2 = "+val2->s << endl;
            }
            else
            {
                string memory2 = (funcName->tempVar.find(val2->s))->second.tiggerName;
                if(memory2[0]!='a') 
                    cout << "\tload "+memory2+" t2" << endl;
                else 
                    cout << "\tt2 = "+memory2 << endl;
            }
        }
        cout << "\tt4 = t1 + t2"<< endl;
        if(val3->isNum)
        {
            cout << "\tt3 = "+val3->s<< endl;
            cout << "\tt4[0] = t3"<< endl;
            break;
        }
        if(IsGlobalVar(val3->s))
        {
            EntryIT it = globalVar.find(val3->s);
            if(it->second.isArray==0) 
                cout << "\tload "+ it->second.tiggerName+"  t3"<< endl;
            else 
                cout << "\tloadaddr "+ it->second.tiggerName+"  t3"<< endl;
        }
        else
        {
            if(val3->isNum)
            {
                cout << "\tt3 = "+val3->s<< endl;
            }
            else
            {
                EntryIT it = (funcName->tempVar.find(val3->s));
                string memory3 = it->second.tiggerName;
                if(memory3[0]!='a')
                {
                    if(it->second.isArray==0)
                        cout << "\tload "+memory3+" t3"<< endl;
                    else
                        cout << "\tloadaddr "+memory3+" t3"<< endl;
                }
                else 
                    cout << "\tt3 = "+memory3<< endl;
            }
        }
        cout << "\tt4[0] = t3"<< endl;
        break;


    case 3:
        if(IsGlobalVar(s2))
        {
            cout << "\tloadaddr "+(globalVar.find(s2))->second.tiggerName+"  t2"<< endl;
        }
        else{
            string memory2 = (funcName->tempVar.find(s2))->second.tiggerName;
            if(memory2[0]!='a') 
                cout << "\tloadaddr "+memory2+" t2"<< endl;
            else 
                cout << "\tt2 = "+memory2<< endl;
        }
        if
        (IsGlobalVar(val3->s))
        {
            cout << "\tload "+(globalVar.find(val3->s))->second.tiggerName+"  t3"<< endl;
        }
        else
        {
            if(val3->isNum)
                cout << "\tt3 = "+val3->s<< endl;
            else
            {
                string memory3 = (funcName->tempVar.find(val3->s))->second.tiggerName;
                if(memory3[0]!='a') 
                    cout << "\tload "+memory3+" t3"<< endl;
                else 
                    cout << "\tt3 = "+memory3<< endl;
            }
        }
        cout << "\tt4 = t2 + t3" << endl;
        if(IsGlobalVar(val1))
        {
            cout << "\tloadaddr "+(globalVar.find(val1))->second.tiggerName+"  t1"<< endl;
            cout << "\tt1[0] = t4[0]"<< endl;
        }
        else{
            string memory1 = (funcName->tempVar.find(val1))->second.tiggerName;
            if(memory1[0]!='a')
            {
                cout << "\tt1 = t4[0]"<< endl;
                cout << "\tstore t1 "+memory1<< endl;
            } 
            else
                cout << "\t"+memory1+" = t4[0]"<< endl;
        }
        break;
    default:
        break;
    }
}


void IfAST::generator()
{
    bool flag1isNum = false; 
    bool flag2isNum = false;
    if(val1->isNum) 
        flag1isNum=true;
    else
    {
        if(IsGlobalVar(val1->s))
        {
            EntryIT it = globalVar.find(val1->s);
            if(it->second.isArray==0) cout << "\tload  "+ it->second.tiggerName+"  t5"<< endl;
            else cout << "\tloadaddr  "+ it->second.tiggerName+"  t5"<< endl;
        } 
        else 
        {
            EntryIT it = (funcName->tempVar.find(val1->s));
            string memory = it->second.tiggerName;
            if(memory[0]!='a')
            {
                if(it->second.isArray==0) 
                    cout << "\tload  "+memory+"  t5"<< endl;
                else 
                    cout << "\tloadaddr  "+memory+"  t5"<< endl;
            } 
            else   
                cout << "\tt5 = "+memory<< endl;
        }
    }

    if(val2->isNum) 
        flag2isNum=true;
    else
    {
        if(IsGlobalVar(val2->s))
        {
            EntryIT it = globalVar.find(val2->s);
            if(it->second.isArray==0) cout << "\tload  "+it->second.tiggerName+"  t6"<< endl;
            else cout << "\tloadaddr  "+it->second.tiggerName+"  t6"<< endl;
        } 
        else 
        {
            EntryIT it = (funcName->tempVar.find(val2->s));
            string memory = it->second.tiggerName;
            if(memory[0]!='a'){
                if(it->second.isArray==0) 
                    cout << "\tload  "+memory+"  t6"<< endl;
                else 
                    cout << "\tloadaddr  "+memory+"  t6"<< endl;
            }
            else   
                cout << "\tt6 = "+memory<< endl;
        }
    }

    if(flag1isNum==true && flag2isNum==true)
    {
        cout << "\tt5 = "+val1->s<< endl;
        cout << "\tt6 = "+val2->s<< endl;
        cout << "\tif  t5 "+ op +" t6  goto "+label<< endl;
        return;
    }
    if(flag1isNum==false && flag2isNum==true)
    {
        cout << "\tt6 = "+val2->s << endl;
        cout << "\tif  t5 "+ op +" t6  goto "+label<< endl;
        return ;
    }
    if(flag1isNum==true && flag2isNum==false)
    {
        cout << "\tt5 = "+val1->s << endl;
        cout << "\tif  t5 "+ op +" t6  goto "+label<< endl;
        return ;
    }
    if(flag1isNum==false && flag2isNum==false)
    {
        cout << "\tif  t5 "+ op +" t6  goto "+label<< endl;
        return ;
    }
}
void GotoAST::generator()
{
    cout << "\tgoto "+label<< endl;
}
void CallAST::generator()
{
    int stackV = funcName->TNum+funcName->tNum+8;
    if(isAssign)
    {
        cout << "\tcall  "+funName<< endl;
        if(IsGlobalVar(leftName))
        {
            cout << "\tloadaddr  "+(globalVar.find(leftName))->second.tiggerName+"  t5"<< endl;
            cout << "\tt5[0] = a0"<< endl;
        } 
        else 
        {
            string memory = (funcName->tempVar.find(leftName))->second.tiggerName;
            if(memory[0]!='a') 
                cout << "\tstore  a0  "+memory<< endl;
            else 
                cout << "\t"+memory+" = a0"<< endl;
        }
    }
    else
    {
        cout << "\tcall  "+funName<< endl;
    }
    for(int i=0;i<nowParam;++i)
    {
        string an = "a"+convert2string(i);
        cout << "\tload  "+convert2string(stackV-i-1)+" "+an<< endl;
    }
    nowParam = 0;

}
void RetAST::generator()
{
    if(isVoid)  
        cout << "\treturn"<< endl;
    else
    {
        if(dynamic_cast<FunHeadAST*>(funcName->head)->funName=="f_main")
        {
            cout << "\ta0 = 10\n\tcall f_putch"<< endl;
        }
        if(retVal->isNum == 0)
        {
            if(IsGlobalVar(retVal->s))
            {
                EntryIT it = globalVar.find(retVal->s);
                if(it->second.isArray==0)cout << "\tload  "+ it->second.tiggerName+"  a0"<< endl;
                else cout << "\tloadaddr  "+ it->second.tiggerName+"  a0"<< endl;
                cout << "\treturn"<< endl;
            } 
            else 
            {
                EntryIT it = (funcName->tempVar.find(retVal->s));
                string memory = it->second.tiggerName;
                if(memory[0]!='a')
                {
                    if(it->second.isArray==0) 
                        cout << "\tload  "+memory+"  a0"<< endl;
                    else 
                        cout << "\tloadaddr  "+memory+"  a0"<< endl;
                } 
                else 
                {
                    cout << "\ta0 = "+memory<< endl;
                }
                cout << "\treturn"<< endl;
            }
        } 
        else
        {
            cout << "\ta0 = "+retVal->s<< endl;
            cout << "\treturn"<< endl;
        }
    }
}
void LabelAST::generator()
{
    cout << Label+":"<< endl;
}
void ParamAST::generator()
{
    int stackV = funcName->TNum+funcName->tNum+8;
    if(nowParam==0){
        for(int i = 0; i < 8; ++i)
        {
            string an = "a"+convert2string(i);
            cout << "\tstore  "+an+" "+convert2string(stackV-i-1)<< endl;
        }
    }

    if(p->isNum)
    {
        cout << "\ta"+convert2string(nowParam)+" = "+p->s<< endl;
    }
    else
    {
        if(IsGlobalVar(p->s))
        {
            EntryIT it = globalVar.find(p->s);
            if(it->second.isArray==0)cout << "\tload  "+ it->second.tiggerName+"  t5"<< endl;
            else cout << "\tloadaddr  "+ it->second.tiggerName+"  t5"<< endl;
            cout << "\ta"+ convert2string(nowParam) + " = t5"<< endl;
        } 
        else 
        {
            EntryIT it = (funcName->tempVar.find(p->s));
            string memory = it->second.tiggerName;
            if(memory[0]!='a')
            {
                if(it->second.isArray==0)cout << "\tload  "+memory+"  t5"<< endl;
                else cout << "\tloadaddr  "+memory+"  t5"<< endl;
            }
            else 
            {
                cout << "\tload  "+convert2string(stackV-(memory[1]-'0')-1)+" t5"<< endl;
            }
            cout << "\ta"+ convert2string(nowParam) + " = t5"<< endl;
        }
    }
    nowParam++;
}
