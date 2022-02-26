#include "eeprint.hpp"
#include <iostream>
#include <list>

std::list<LineOfEEyore> eelines;

void EEDump(const LineOfEEyore &p)
{
	using std::cout;
	using std::endl;
	using std::string;
	EERecord type_ = p.type;
	//def:enum class EERecord{DefVar, Header, End, BinExpr, SingExpr, Copy, LArr, RArr, Cond, Uncond, Label, Param, Voidcall, Asscall, Voidret, Ret};

	if(type_ == EERecord::DefVar)
	{
		cout << "var ";
		if(!p.num.empty())
			cout << p.num << " ";
		cout << p.sym[0] << endl;
	}

	if(type_ == EERecord::Header)
	{
		cout << p.sym[0] << " [" << p.num << "]" << endl;
	}

	if(type_ == EERecord::End)
	{
		cout << "end " << p.sym[0] << endl;
	}
	if(type_ == EERecord::BinExpr)
	{
		if(p.label.empty()) // Arithmetic Op
			cout << p.sym[0] << " = " << p.sym[1] << " " << p.op << " " << p.sym[2] << endl;
			else
			{
				cout << "if " << p.sym[1] << " " << p.op << " " << p.sym[2] << " goto " << p.label << endl;
				cout << p.sym[0] << " = 0" << endl;
				cout << "goto " << p.num << endl;
				cout << p.label << ":" << endl;
				cout << p.sym[0] << " = 1" << endl;
				cout << p.num << ":" << endl;
			}
	}

	if(type_ == EERecord::SingExpr)
	{
		cout << p.sym[0] << " = " << p.op << " " << p.sym[1] << endl;
	}	

	if(type_ == EERecord::Copy)
	{
		cout << p.sym[0] << " = " << p.sym[1] << endl;
	}	

	if(type_ == EERecord::LArr)
	{
		cout << p.sym[0] << "[" << p.sym[1] << "] = " << p.sym[2] << endl;
	}	

	if(type_ == EERecord::RArr)
	{
		cout << p.sym[0] << " = " << p.sym[1] << "[" << p.sym[2] << "]" << endl;
	}	

	if(type_ == EERecord::Cond)
	{
		cout << "if " << p.sym[0] << " " << p.op << " " << p.sym[1] << " goto " << p.label << endl;
	}	

	if(type_ == EERecord::Uncond)
	{
		cout << "goto " << p.label << endl;	
	}	

	if(type_ == EERecord::Label)
	{
		cout << p.label << ":" << endl;
	}	

	if(type_ == EERecord::Param)
	{
			cout << "param " << p.sym[0] << endl;
	}	

	if(type_ == EERecord::Voidcall)
	{
		cout << "call " << p.sym[0] << endl;
	}	

	if(type_ == EERecord::Asscall)
	{
		cout << p.sym[0] << " = call " << p.sym[1] << endl;
	}	

	if(type_ == EERecord::Voidret)
	{
		cout << "return" << endl;
	}

	if(type_ == EERecord::Ret)
	{
		cout << "return " << p.sym[0] << endl;		
	}	
	
}
