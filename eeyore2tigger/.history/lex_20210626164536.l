%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string>
    #include <iostream>
    #include <sstream>
    #include "tiggerbasic.hpp"
    #include "parser.tab.hpp"
    using namespace::std;
    extern "C" int input();
%}
%option noyywrap
Space           [ \t]+
vari             [tTp][0-9]+
Function        f\_[a-zA-Z0-9\_]+
digit           (-)?[0-9]+
op              [\+\-\!\*\/\%\>\<]|&{2}|\|{2}|={2}|\>=|\<=|!=
label           l[0-9]+

%%
{Space}         {;}
"\n"            {yylineno++;}
"\r"            {yylineno++;}
\/\/.*\n?       {for(auto i : string(yytext_ptr)) { if (i == '\n') yylineno++; } }

{vari}           {yylval.token = new Token(symbol, yytext,yytext[0] == 't'); return SYMBOL;}

{Function}      {yylval.token = new Token(functiontype, yytext, false);return FUNCTION;}

{digit}         {yylval.token = new Token(num,yytext, false);return NUM;}
{label}         {yylval.token = new Token(label,yytext, false);return LABEL;}

"var"           {return VAR;}
"end"           {return END;}
"param"         {return PARAM;}
"if"            {return IF;}
"goto"          {return GOTO;}
"call"          {return CALL;}
"return"        {return RETURN;}
"<="		    {yylval.token = new Token(op, LE, false); return LE;}
">="		    {yylval.token = new Token(op, GE, false); return GE;}
"=="		    {yylval.token = new Token(op, EQ, false); return EQ;}
"!="		    {yylval.token = new Token(op, NE, false); return NE;}
{op}            {yylval.token = new Token(op, yytext, false);return OP;}
"="             {yylval.token = new Token(op, yytext, false);return ASSIGN;}
"["             {return LCOMMA;}
"]"             {return RCOMMA;}
":"             {return COLON;}

%%
