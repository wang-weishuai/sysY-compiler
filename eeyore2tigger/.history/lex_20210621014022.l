%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string>
    #include <iostream>
    #include <sstream>
    #include "tiggerbasic.hpp"
    #include "parser.tab.hpp"
    using namespace::std;
    inline void handleCommentLine(){
        for(auto i : string(yytext_ptr)) { if (i == '\n') yylineno++; }
    }
%}
%option noyywrap

Space          " "|\t
t_var           t[0-9]+
T_var           T[0-9]+
p_var           p[0-9]+
Function       f_[a-zA-Z_][a-zA-Z_0-9]*

digit           [0-9]
digit_int       (-)?[0-9]+

op              [\+\-\!\*\/\%\>\<]
label           l[0-9]+


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



"var"           {return VAR;}
"end"           {return END;}
"param"         {return PARAM;}
"if"            {return IF;}
"goto"          {return GOTO;}
"call"          {return CALL;}
"return"        {return RETURN;}

{op}            {yylval.token = new Token(op, yytext, false);return OP;}
"="             {yylval.token = new Token(op, yytext, false);return ASSIGN;}
"["             {return LCOMMA;}
"]"             {return RCOMMA;}
":"             {return COLON;}

%%