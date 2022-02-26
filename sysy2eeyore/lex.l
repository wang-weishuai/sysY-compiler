%{
#include <algorithm>
#include "global.hpp"
#include "eebasic.hpp"
#include "parser.tab.hpp"
static int Convert2int(const char *);
#ifdef LEX_L
YYSTYPE yylval;
#endif
%}

%option noyywrap

letter_	[_a-zA-Z]
digit		[0-9]
hexdigit	digit[a-fA-F]
singleop [\+\-\!\*\/\%\>\<]
seperator	[\,\;\[\]\=\{\}\(\)]
ident		{letter_}({letter_}|{digit})*
decimal		[1-9]{digit}*
hexadecimal	0[xX]{hexdigit}+
octal		0{digit}*
integer		({decimal}|{hexadecimal}|{octal})
splitor       [ \n\t\r]+
lcomment	\/\/.*
bcomment	\/\*([^\*]|(\*)*[^\*/])*(\*)*\*\/
%%

{singleop}  {
				yylval.tkptr = new Token(*yytext);
				return *yytext;
			}

{seperator} {
				yylval.tkptr = new Token(*yytext);
				return *yytext;
			}

"const"		{
				yylval.tkptr = new Token(CONST);
				return CONST;
			}

"int"		{
				yylval.tkptr = new Token(INT);
				return INT;
			}

"void"		{
				yylval.tkptr = new Token(VOID);
				return VOID;
			}

"if"		{
				yylval.tkptr = new Token(IF);
				return IF;
			}

"else"		{
				yylval.tkptr = new Token(ELSE);
				return ELSE;
			}

"while"		{
				yylval.tkptr = new Token(WHILE);
				return WHILE;
			}

"break"		{
				yylval.tkptr = new Token(BREAK);
				return BREAK;
			}

"continue"	{
				yylval.tkptr = new Token(CONTINUE);
				return CONTINUE;
			}

"return"	{
				yylval.tkptr = new Token(RETURN);
				return RETURN;
			}

"<="		{
				yylval.tkptr = new Token(LE);
				return LE;
			}

">="		{
				yylval.tkptr = new Token(GE);
				return GE;
			}

"=="		{
				yylval.tkptr = new Token(EQ);
				return EQ;
			}

"!="		{
				yylval.tkptr = new Token(NE);
				return NE;
			}

"&&"		{
				yylval.tkptr = new Token(AND);
				return AND;
			}

"||"		{
				yylval.tkptr = new Token(OR);
				return OR;
			}

{ident}		{
				yylval.tkptr = new Token(yytext, IDENT);
				return IDENT;
			}

{integer}	{
				yylval.tkptr = new Token(Convert2int(yytext), INT_CONST);
				return INT_CONST;
			}

{splitor}|{lcomment}|{bcomment}			{;}

%%


int Convert2int(const char *yytext)
{ 	// May cause some problem.
	if(yytext[0] == '0' && yyleng == 1) return 0;
	else if(yytext[0] == '0' && (yytext[1] == 'x' || yytext[1] == 'X'))
		return std::stoi(yytext, nullptr, 16);
	else if(yytext[0] == '0' && yytext[1] - '0' > 0 && yytext[1] - '0' <= 7)
		return std::stoi(yytext, nullptr, 8);
	return std::stoi(yytext);
}
