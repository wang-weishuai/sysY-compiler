%{
#include "globals.h"
#include "util.h"
#include "Tigger2RISCV.tab.hpp"
#include <stdio.h>

extern FILE* RISCV_code;
%}
letter		[a-zA-Z_]
digit		[0-9]
int 		{digit}+
id	{letter}+{digit}*{letter}*{digit}*
newline		\n
whitespace	[  \t]+

var          v[0-9]+|"x0"|s[0-9]|"s10"|"s11"|t[0-6]|a[0-7]
label		"l"{int}

%%
{var}			{
					yylval = NewNode(Var_K);
					yylval->attr.Var_info.index = atoi(yytext + 1);
					yylval->attr.Var_info.kind = yytext[0];
					return VAR;
				}

{label}			{
					yylval = NewNode(Var_K);
					yylval->attr.Var_info.index = atoi(yytext + 1);
					yylval->attr.Var_info.kind = yytext[0];
					return LABEL;
				}

"f_"{id} 		{
					yylval = newFuncNode();
					yylval->attr.func_name = copyString(yytext);
					return FUN;
				}

"end"			{return END;}
"call"			{return CALL;}
"if"			{return IF;}
"goto"			{return GOTO;}
"return"		{return RETURN;}
"store"			{return STORE;}
"load"			{return LOAD;}
"loadaddr"		{return LOADADDR;}
"malloc"		{return MALLOC;}
"=="			{return EQ;}
"!="			{return NEQ;}
"&&" 			{return AND;}
"||"			{return OR;}
"-"{int}		{yylval = NewNode(Const_K);
				 yylval->attr.val = atoi(yytext);
				 return INT;}
{int}			{yylval = NewNode(Const_K);
				 yylval->attr.val = atoi(yytext);
				 return INT;}
{newline}		{lineno++;}
{whitespace}	{}
"//"			{ char c;
				  do
				  {
					  c = getchar();
					  if (c == '\n') lineno++;
				  } while (c != '\n');
				}
[\+\-\*\/\=\!\%\>\<\(\)\[\]\{\}\:]	{return yytext[0];}

%%

int yywrap()
{
	return 1;
}
