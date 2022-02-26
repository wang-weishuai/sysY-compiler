#include "tiggerbasic.hpp"
#include <iostream>
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
      for(auto i : globalDecl){
        if(dynamic_cast<DefVarAST* >(i)->isArray){
            Table tmp("v"+convert2string(vn++), true, dynamic_cast<DefVarAST* >(i)->num);
            globalVar.insert(pair<string,Table>(dynamic_cast<DefVarAST* >(i)->varName,tmp));
        }else {
            Table tmp("v"+convert2string(vn++), false,0);
            globalVar.insert(pair<string,Table>(dynamic_cast<DefVarAST* >(i)->varName,tmp));
        }
    }
    for(auto i : globalInit){
        if(dynamic_cast<InitAST* >(i)->isArray) continue;
        
        EntryIT it = globalVar.find(dynamic_cast<InitAST* >(i)->leftName);
        it->second.val = dynamic_cast<InitAST* >(i)->rightNum;
        
    }
      for(auto i : globalVar){
        if(i.second.isArray==false){
            tiggerCode.push_back(i.second.tiggerName + " = " + convert2string(i.second.val));
        }else{
            tiggerCode.push_back(i.second.tiggerName + " = malloc " + convert2string(i.second.arrNum));
        }
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
    tiggerCode.push_back(funName+" ["+convert2string(paraNum)+"]"+ " ["+convert2string(funcName->tNum+funcName->TNum + 8)+"]");
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
    tiggerCode.push_back("end "+funName);
}

void BinaryAST::generator()
{
    bool flag2isNum = false;
    if(firstVal->isNum) 
        tiggerCode.push_back("\tt5 = "+firstVal->s);
    else
    {
        if(IsGlobalVar(firstVal->s))
        {
            EntryIT it = globalVar.find(firstVal->s);
            if(it->second.isArray == 0)
                tiggerCode.push_back("\tload  "+it->second.tiggerName+"  t5");
            else
                tiggerCode.push_back("\tloadaddr  "+it->second.tiggerName+"  t5");
        } 
        else 
        {
            EntryIT it = funcName->tempVar.find(firstVal->s);
            string memory = it->second.tiggerName;
            if(memory[0]!='a')
            {
                if(it->second.isArray==0)tiggerCode.push_back("\tload  "+memory+"  t5");
                else{tiggerCode.push_back("\tloadaddr  "+memory+"  t5");}
            }  
            else    
                tiggerCode.push_back("\tt5 = "+memory);
        }
    }

    if(secVal->isNum) 
        flag2isNum = true;
    else
    {
        if(IsGlobalVar(secVal->s))
        {
            EntryIT it = globalVar.find(secVal->s);
            if(it->second.isArray==0) tiggerCode.push_back("\tload  "+it->second.tiggerName+"  t6");
            else {tiggerCode.push_back("\tloadaddr  "+it->second.tiggerName+"  t6");}
        } 
        else 
        {
            EntryIT it = funcName->tempVar.find(secVal->s);
            string memory = it->second.tiggerName;
            if(memory[0]!='a'){
                if(it->second.isArray==0) 
                    tiggerCode.push_back("\tload  "+memory+"  t6");
                else 
                    tiggerCode.push_back("\tloadaddr  "+memory+"  t6");   
            }
            else    tiggerCode.push_back("\tt6 = "+memory);
        }
    }

    if(IsGlobalVar(leftName))
    {
        if(flag2isNum==false)   
            tiggerCode.push_back("\tt4 = t5 "+op+" t6");
        else    
            tiggerCode.push_back("\tt4 = t5 "+op+" "+secVal->s);

        tiggerCode.push_back("\tloadaddr  "+(globalVar.find(leftName))->second.tiggerName+"  t3");
        tiggerCode.push_back("\tt3[0] = t4");
    }
    else
    {
        if(flag2isNum==false)   
            tiggerCode.push_back("\tt4 = t5 "+op+" t6");
        else    
            tiggerCode.push_back("\tt4 = t5 "+op+" "+secVal->s);

        string memoryLeft = (funcName->tempVar.find(leftName))->second.tiggerName;
        if(memoryLeft[0]!='a') 
            tiggerCode.push_back("\tstore t4 "+ memoryLeft);
        else 
            tiggerCode.push_back("\t"+memoryLeft+" = t4");
    }
    
}


void SingleAST::generator()
{
    if(firstVal->isNum) 
    {
        tiggerCode.push_back("\tt5 = "+firstVal->s);
    }
    else
    {
        if(IsGlobalVar(firstVal->s))
        {
            EntryIT it = globalVar.find(firstVal->s);
            if(it->second.isArray==0)
                tiggerCode.push_back("\tload  "+it->second.tiggerName+"  t5");
            else
            {
                tiggerCode.push_back("\tloadaddr  "+it->second.tiggerName+"  t5");
            }
        } 
        else   
        {
            EntryIT it = funcName->tempVar.find(firstVal->s);
            string memory = it->second.tiggerName;
            if(memory[0]!='a'){
                if(it->second.isArray==0) 
                    tiggerCode.push_back("\tload  "+memory+"  t5");
                else 
                {
                    tiggerCode.push_back("\tloadaddr  "+memory+"  t5");
                }
            }
            else tiggerCode.push_back("\tt5 = "+memory);
        }
    }
    tiggerCode.push_back("\tt4 = "+op+" t5 ");
    if(IsGlobalVar(leftName))
    {
        tiggerCode.push_back("\tloadaddr  "+(globalVar.find(leftName))->second.tiggerName+"  t3");
        tiggerCode.push_back("\tt3[0] = t4");
    }
    else
    {
        string memory = (funcName->tempVar.find(leftName))->second.tiggerName;
        if(memory[0]!='a') 
            tiggerCode.push_back("\tstore t4 "+ memory );
        else 
            tiggerCode.push_back("\t"+memory+" = t4");
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
                tiggerCode.push_back("\tt4 = "+val2->s);
                tiggerCode.push_back("\tloadaddr  "+(globalVar.find(val1))->second.tiggerName+"  t3");
                tiggerCode.push_back("\tt3[0] = t4");
            }
            else
            {
                string memory = (funcName->tempVar.find(val1))->second.tiggerName;
                tiggerCode.push_back("\tt5 = "+val2->s);
                if(memory[0]!='a') 
                    tiggerCode.push_back("\tstore t5 "+memory);
                else 
                    tiggerCode.push_back("\t"+memory+" = t5");
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
                        tiggerCode.push_back("\tload  "+ it->second.tiggerName+"  t4");
                    else 
                        tiggerCode.push_back("\tloadaddr  "+ it->second.tiggerName+"  t4");
                }
                else
                {
                    EntryIT it = funcName->tempVar.find(val2->s);
                    string memory = it->second.tiggerName;
                    if(memory[0]!='a')
                    {
                        if(it->second.isArray==0)
                            tiggerCode.push_back("\tload  "+memory+"  t4");
                        else 
                            tiggerCode.push_back("\tloadaddr  "+memory+"  t4");
                    }
                    else
                        tiggerCode.push_back("\tt4 = "+memory);
                }
                tiggerCode.push_back("\tloadaddr  "+(globalVar.find(val1))->second.tiggerName+"  t3");
                tiggerCode.push_back("\tt3[0] = t4");
            }
            else
            {
                if(IsGlobalVar(val2->s)){
                    EntryIT it = globalVar.find(val2->s);
                    if(it->second.isArray==0)
                        tiggerCode.push_back("\tload  "+ it->second.tiggerName+"  t4");
                    else    
                        tiggerCode.push_back("\tloadaddr  "+ it->second.tiggerName+"  t4");
                }
                else
                {
                    EntryIT it = (funcName->tempVar.find(val2->s));
                    string memory = it->second.tiggerName;
                    if(memory[0]!='a')
                    {
                        if(it->second.isArray==0)
                            tiggerCode.push_back("\tload  "+memory+"  t4");
                        else
                            tiggerCode.push_back("\tloadaddr  "+memory+"  t4");
                    }
                    else 
                        tiggerCode.push_back("\tt4 = "+memory);
                }
                string memoryLeft = (funcName->tempVar.find(val1))->second.tiggerName;
                if(memoryLeft[0]!='a') 
                    tiggerCode.push_back("\tstore t4 "+memoryLeft);
                else 
                    tiggerCode.push_back("\t"+memoryLeft+" = t4");
            }
        }
        break;


    case 2:
  
        if(IsGlobalVar(val1))
        {
            tiggerCode.push_back("\tloadaddr "+(globalVar.find(val1))->second.tiggerName+"  t1");
        }
        else
        {
            string memory1 = (funcName->tempVar.find(val1))->second.tiggerName;
            if(memory1[0]!='a') 
                tiggerCode.push_back("\tloadaddr "+memory1+" t1");
            else
                tiggerCode.push_back("\tt1 = "+memory1);
        }
        if(IsGlobalVar(val2->s))
        {
            tiggerCode.push_back("\tload "+(globalVar.find(val2->s))->second.tiggerName+"  t2");
        }
        else
        {
            if(val2->isNum)
            {
                tiggerCode.push_back("\tt2 = "+val2->s);
            }
            else
            {
                string memory2 = (funcName->tempVar.find(val2->s))->second.tiggerName;
                if(memory2[0]!='a') 
                    tiggerCode.push_back("\tload "+memory2+" t2");
                else 
                    tiggerCode.push_back("\tt2 = "+memory2);
            }
        }
        tiggerCode.push_back("\tt4 = t1 + t2");
        if(val3->isNum)
        {
            tiggerCode.push_back("\tt3 = "+val3->s);
            tiggerCode.push_back("\tt4[0] = t3");
            break;
        }
        if(IsGlobalVar(val3->s))
        {
            EntryIT it = globalVar.find(val3->s);
            if(it->second.isArray==0) 
                tiggerCode.push_back("\tload "+ it->second.tiggerName+"  t3");
            else 
                tiggerCode.push_back("\tloadaddr "+ it->second.tiggerName+"  t3");
        }
        else
        {
            if(val3->isNum)
            {
                tiggerCode.push_back("\tt3 = "+val3->s);
            }
            else
            {
                EntryIT it = (funcName->tempVar.find(val3->s));
                string memory3 = it->second.tiggerName;
                if(memory3[0]!='a')
                {
                    if(it->second.isArray==0)
                        tiggerCode.push_back("\tload "+memory3+" t3");
                    else
                        tiggerCode.push_back("\tloadaddr "+memory3+" t3");
                }
                else 
                    tiggerCode.push_back("\tt3 = "+memory3);
            }
        }
        tiggerCode.push_back("\tt4[0] = t3");
        break;


    case 3:
        if(IsGlobalVar(s2))
        {
            tiggerCode.push_back("\tloadaddr "+(globalVar.find(s2))->second.tiggerName+"  t2");
        }
        else{
            string memory2 = (funcName->tempVar.find(s2))->second.tiggerName;
            if(memory2[0]!='a') 
                tiggerCode.push_back("\tloadaddr "+memory2+" t2");
            else 
                tiggerCode.push_back("\tt2 = "+memory2);
        }
        if
        (IsGlobalVar(val3->s))
        {
            tiggerCode.push_back("\tload "+(globalVar.find(val3->s))->second.tiggerName+"  t3");
        }
        else
        {
            if(val3->isNum)
                tiggerCode.push_back("\tt3 = "+val3->s);
            else
            {
                string memory3 = (funcName->tempVar.find(val3->s))->second.tiggerName;
                if(memory3[0]!='a') 
                    tiggerCode.push_back("\tload "+memory3+" t3");
                else 
                    tiggerCode.push_back("\tt3 = "+memory3);
            }
        }
        tiggerCode.push_back("\tt4 = t2 + t3");
        if(IsGlobalVar(val1))
        {
            tiggerCode.push_back("\tloadaddr "+(globalVar.find(val1))->second.tiggerName+"  t1");
            tiggerCode.push_back("\tt1[0] = t4[0]");
        }
        else{
            string memory1 = (funcName->tempVar.find(val1))->second.tiggerName;
            if(memory1[0]!='a')
            {
                tiggerCode.push_back("\tt1 = t4[0]");
                tiggerCode.push_back("\tstore t1 "+memory1);
            } 
            else
                tiggerCode.push_back("\t"+memory1+" = t4[0]");
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
            if(it->second.isArray==0) tiggerCode.push_back("\tload  "+ it->second.tiggerName+"  t5");
            else{tiggerCode.push_back("\tloadaddr  "+ it->second.tiggerName+"  t5");}
        } 
        else 
        {
            EntryIT it = (funcName->tempVar.find(val1->s));
            string memory = it->second.tiggerName;
            if(memory[0]!='a')
            {
                if(it->second.isArray==0) 
                    tiggerCode.push_back("\tload  "+memory+"  t5");
                else 
                    tiggerCode.push_back("\tloadaddr  "+memory+"  t5");
            } 
            else   
                tiggerCode.push_back("\tt5 = "+memory);
        }
    }

    if(val2->isNum) 
        flag2isNum=true;
    else
    {
        if(IsGlobalVar(val2->s))
        {
            EntryIT it = globalVar.find(val2->s);
            if(it->second.isArray==0) tiggerCode.push_back("\tload  "+it->second.tiggerName+"  t6");
            else{tiggerCode.push_back("\tloadaddr  "+it->second.tiggerName+"  t6");}
        } 
        else 
        {
            EntryIT it = (funcName->tempVar.find(val2->s));
            string memory = it->second.tiggerName;
            if(memory[0]!='a'){
                if(it->second.isArray==0) 
                    tiggerCode.push_back("\tload  "+memory+"  t6");
                else 
                    tiggerCode.push_back("\tloadaddr  "+memory+"  t6");
            }
            else   
                tiggerCode.push_back("\tt6 = "+memory);
        }
    }

    if(flag1isNum==true && flag2isNum==true)
    {
        tiggerCode.push_back("\tt5 = "+val1->s);
        tiggerCode.push_back("\tt6 = "+val2->s);
        tiggerCode.push_back("\tif  t5 "+ op +" t6  goto "+label);
        return;
    }
    if(flag1isNum==false && flag2isNum==true)
    {
        tiggerCode.push_back("\tt6 = "+val2->s);
        tiggerCode.push_back("\tif  t5 "+ op +" t6  goto "+label);
        return ;
    }
    if(flag1isNum==true && flag2isNum==false)
    {
        tiggerCode.push_back("\tt5 = "+val1->s);
        tiggerCode.push_back("\tif  t5 "+ op +" t6  goto "+label);
        return ;
    }
    if(flag1isNum==false && flag2isNum==false)
    {
        tiggerCode.push_back("\tif  t5 "+ op +" t6  goto "+label);
        return ;
    }
}
void GotoAST::generator()
{
    tiggerCode.push_back("\tgoto "+label);
}
void CallAST::generator()
{
    int stackV = funcName->TNum+funcName->tNum+8;
    if(isAssign)
    {
        tiggerCode.push_back("\tcall  "+funName);
        if(IsGlobalVar(leftName))
        {
            tiggerCode.push_back("\tloadaddr  "+(globalVar.find(leftName))->second.tiggerName+"  t5");
            tiggerCode.push_back("\tt5[0] = a0");
        } 
        else 
        {
            string memory = (funcName->tempVar.find(leftName))->second.tiggerName;
            if(memory[0]!='a') 
                tiggerCode.push_back("\tstore  a0  "+memory);
            else 
                tiggerCode.push_back("\t"+memory+" = a0");
        }
    }
    else
    {
        tiggerCode.push_back("\tcall  "+funName);
    }
    for(int i=0;i<nowParam;++i)
    {
        string an = "a"+convert2string(i);
        tiggerCode.push_back("\tload  "+convert2string(stackV-i-1)+" "+an);
    }
    nowParam = 0;

}
void RetAST::generator()
{
    if(isVoid)  
        tiggerCode.push_back("\treturn");
    else
    {
        if(dynamic_cast<FunHeadAST*>(funcName->head)->funName=="f_main")
        {
            tiggerCode.push_back("\ta0 = 10\n\tcall f_putch");
        }
        if(retVal->isNum == 0)
        {
            if(IsGlobalVar(retVal->s))
            {
                EntryIT it = globalVar.find(retVal->s);
                if(it->second.isArray==0)tiggerCode.push_back("\tload  "+ it->second.tiggerName+"  a0");
                else{tiggerCode.push_back("\tloadaddr  "+ it->second.tiggerName+"  a0");}
                tiggerCode.push_back("\treturn");
            } 
            else 
            {
                EntryIT it = (funcName->tempVar.find(retVal->s));
                string memory = it->second.tiggerName;
                if(memory[0]!='a')
                {
                    if(it->second.isArray==0) 
                        tiggerCode.push_back("\tload  "+memory+"  a0");
                    else 
                        tiggerCode.push_back("\tloadaddr  "+memory+"  a0");
                } 
                else 
                {
                    tiggerCode.push_back("\ta0 = "+memory);
                }
                tiggerCode.push_back("\treturn");
            }
        } 
        else
        {
            tiggerCode.push_back("\ta0 = "+retVal->s);
            tiggerCode.push_back("\treturn");
        }
    }
}
void LabelAST::generator()
{
    tiggerCode.push_back(Label+":");
}
void ParamAST::generator()
{
    int stackV = funcName->TNum+funcName->tNum+8;
    if(nowParam==0){
        for(int i = 0; i < 8; ++i)
        {
            string an = "a"+convert2string(i);
            tiggerCode.push_back("\tstore  "+an+" "+convert2string(stackV-i-1));
        }
    }

    if(p->isNum)
    {
        tiggerCode.push_back("\ta"+convert2string(nowParam)+" = "+p->s);
    }
    else
    {
        if(IsGlobalVar(p->s))
        {
            EntryIT it = globalVar.find(p->s);
            if(it->second.isArray==0)tiggerCode.push_back("\tload  "+ it->second.tiggerName+"  t5");
            else {tiggerCode.push_back("\tloadaddr  "+ it->second.tiggerName+"  t5");}
            tiggerCode.push_back("\ta"+ convert2string(nowParam) + " = t5");
        } 
        else 
        {
            EntryIT it = (funcName->tempVar.find(p->s));
            string memory = it->second.tiggerName;
            if(memory[0]!='a')
            {
                if(it->second.isArray==0)tiggerCode.push_back("\tload  "+memory+"  t5");
                else tiggerCode.push_back("\tloadaddr  "+memory+"  t5");
            }
            else 
            {
                tiggerCode.push_back("\tload  "+convert2string(stackV-(memory[1]-'0')-1)+" t5");
            }
            tiggerCode.push_back("\ta"+ convert2string(nowParam) + " = t5");
        }
    }
    nowParam++;
}
