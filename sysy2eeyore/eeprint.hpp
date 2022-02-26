
#pragma once

#include <string>
#include <list>

enum class EERecord{DefVar, Header, End, BinExpr, SingExpr, Copy, LArr, RArr, Cond, Uncond, Label, Param, Voidcall, Asscall, Voidret, Ret};

class LineOfEEyore{
public:
	LineOfEEyore(EERecord t):type(t) {}
	LineOfEEyore
	(EERecord t, std::string &&p): type(t) 
	{
		if(t == EERecord::Ret || t == EERecord::Param || t == EERecord::Voidcall || t == EERecord::End || t == EERecord::DefVar) sym[0] = p;
		if(t == EERecord::Uncond || t == EERecord::Label) label = p;
	}
	LineOfEEyore(EERecord t, std::string &&p, std::string &&q): type(t) 
	{
		if (t == EERecord::DefVar || t == EERecord::Header) 
		{
			sym[0] = p;
			num = q;
		}
		if (t == EERecord::Copy || t == EERecord::Asscall) 
		{
			sym[0] = p; 
			sym[1] = q;
		}
	}
	LineOfEEyore(EERecord t, std::string &&p, std::string &&q, std::string &&r):type(t) 
	{
		if(t == EERecord::SingExpr) 
		{
			sym[0] = p; 
			sym[1] = q; 
			op = r;
		}
		if(t == EERecord::LArr || t == EERecord::RArr) 
		{
			sym[0] = p; 
			sym[1] = q;
			sym[2] = r;
		}
	}
	LineOfEEyore(EERecord t, std::string &&p, std::string &&q, std::string &&r, std::string &&s):type(t) 
	{
		if(t == EERecord::BinExpr)   
		{
			sym[0] = p; 
			sym[1] = q;
			sym[2] = r; 
			op = s;
		}
		if(t == EERecord::Cond) 
		{
			sym[0] = p; 
			sym[1] = q;
			op = r; 
			label = s;
		}
	}

	EERecord type;
	std::string sym[3];
	std::string op;
	std::string label;
	std::string num;
};

void EEDump(const LineOfEEyore &);

extern std::list<LineOfEEyore> eelines;

