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
identifier	{letter}+{digit}*{letter}*{digit}*
newline		\n
whitespace	[  \t]+

kindofvar   [vtsax]
var         kindofvar{int}
label		"l"{int}

%%
{var}			{
				char var_kind = var[0];
				yylval = NewNode(Var_K);
				 yylval->attr.Var_info.index = atoi(yytext + 1);
				 yylval->attr.Var_info.kind = var_kind;
				 return VAR;}

{label}			{yylval = NewNode(Var_K);
				 yylval->attr.Var_info.index = atoi(yytext + 1);
				 yylval->attr.Var_info.kind = 'l';
				 return LABEL;}
"f_"{identifier} 		{yylval = newFuncNode();
				 yylval->attr.func_name = copyString(yytext);
				 return FUN;}
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
