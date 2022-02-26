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
Space           [ \n\t\r]+
var             [t|T|p][0-9]+
Function        f_[a-zA-Z_][a-zA-Z_0-9]*
digit           (-)?[0-9]+
op              [\+\-\!\*\/\%\>\<]|&{2}|\|{2}|={2}
label           l[0-9]+

%%
{Space}    {;}
"\n"    {yylineno++;}
"\r"    {yylineno++;}
\/\/.*\n?   {
    for(auto i : string(yytext_ptr)) { if (i == '\n') yylineno++; }
}

{var}      {
    yylval.token = new Token(symbol, yytext,yytext[0] == 't');
    return SYMBOL;
}

{Function}  {
    yylval.token = new Token(functiontype, yytext);
    return FUNCTION;
}

{digit} {
    yylval.token = new Token(num,yytext);
    return NUM;
}
{label}     {
    yylval.token = new Token(label,yytext);
    return LABEL;
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