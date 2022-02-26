%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string>
    #include <iostream>
    #include <sstream>
    #include "ast_eeyore.hpp"
    #include "parser.tab.hpp"
    using namespace::std;
    extern "C" int input();
%}
%option noyywrap
Space          " "|\t
t_var           t[0-9]+
T_var           T[0-9]+
p_var           p[0-9]+
Function        f_[a-zA-Z_][a-zA-Z_0-9]*

digit           [0-9]
digit_int       (-)?[0-9]+

op              [\+\-\!\*\/\%\>\<]
label           l[0-9]+


%%
{Space}    {;}
"\n"    {if(Debug) printf("above yylineno = %d.\n",yylineno); yylineno++;}
"\r"    {if(Debug) printf("above yylineno = %d.\n",yylineno); yylineno++;}
\/\/.*\n?   {
    for(auto i : string(yytext_ptr)) { if (i == '\n') yylineno++; }
}

{t_var}    {
    yylval.token = new Token(symbol, yytext,true);
    return SYMBOL;
}
{T_var}    {
    yylval.token = new Token(symbol, yytext,false);
    return SYMBOL;
}
{p_var}    {
    yylval.token = new Token(symbol, yytext,false);
    return SYMBOL;
}
{Function}  {
    yylval.token = new Token(functiontype, yytext);
    return FUNCTION;
}

{digit_int} {
    yylval.token = new Token(num,yytext);
    return NUM;
}
{label}     {
    yylval.token = new Token(label,yytext);
    return LABEL;
}

"&&"        {
    yylval.token = new Token(op, yytext);
    return OP;
}
"||"        {
    yylval.token = new Token(op, yytext);
    return OP;
}
">="        {
    yylval.token = new Token(op, yytext);
    return OP;
}
"<="        {
    yylval.token = new Token(op, yytext);
    return OP;
}
"=="        {
    yylval.token = new Token(op, yytext);
    return OP;
}

"!="        {
    yylval.token = new Token(op, yytext);
    return OP;
}

"var"   {    return VAR;    }
"end"   {   return END;     }
"param" {    return PARAM;    }
"if"    {    return IF;     }
"goto"  {    return GOTO;   }
"call"  {    return CALL;}
"return"    {    return RETURN; }

{op}        {
    yylval.token = new Token(op, yytext);
    return OP;
}
"="     {
    yylval.token = new Token(op, yytext);
    return ASSIGN;
}
"["     { return LCOMMA;}
"]"     { return RCOMMA;}
":"     { return COLON;}

.   {
    printf("Unknown token: %s\n", yytext);
}

%%