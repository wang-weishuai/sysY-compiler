/* A Bison parser, made by GNU Bison 3.7.6.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_TAB_HPP_INCLUDED
# define YY_YY_PARSER_TAB_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    FUNCTION = 258,                /* FUNCTION  */
    OP = 259,                      /* OP  */
    NUM = 260,                     /* NUM  */
    LABEL = 261,                   /* LABEL  */
    ASSIGN = 262,                  /* ASSIGN  */
    VAR = 263,                     /* VAR  */
    PARAM = 264,                   /* PARAM  */
    SYMBOL = 265,                  /* SYMBOL  */
    END = 266,                     /* END  */
    IF = 267,                      /* IF  */
    GOTO = 268,                    /* GOTO  */
    CALL = 269,                    /* CALL  */
    RETURN = 270,                  /* RETURN  */
    LCOMMA = 271,                  /* LCOMMA  */
    RCOMMA = 272,                  /* RCOMMA  */
    COLON = 273                    /* COLON  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define FUNCTION 258
#define OP 259
#define NUM 260
#define LABEL 261
#define ASSIGN 262
#define VAR 263
#define PARAM 264
#define SYMBOL 265
#define END 266
#define IF 267
#define GOTO 268
#define CALL 269
#define RETURN 270
#define LCOMMA 271
#define RCOMMA 272
#define COLON 273

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "parser.y"

    Token* token;
    BaseEeyoreAST* ast;

#line 108 "parser.tab.hpp"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (BaseEeyoreAST* *root);

#endif /* !YY_YY_PARSER_TAB_HPP_INCLUDED  */
