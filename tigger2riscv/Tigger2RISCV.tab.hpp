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

#ifndef YY_YY_TIGGER2RISCV_TAB_HPP_INCLUDED
# define YY_YY_TIGGER2RISCV_TAB_HPP_INCLUDED
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
    VAR = 258,                     /* VAR  */
    LABEL = 259,                   /* LABEL  */
    IF = 260,                      /* IF  */
    GOTO = 261,                    /* GOTO  */
    STORE = 262,                   /* STORE  */
    LOAD = 263,                    /* LOAD  */
    LOADADDR = 264,                /* LOADADDR  */
    MALLOC = 265,                  /* MALLOC  */
    FUN = 266,                     /* FUN  */
    END = 267,                     /* END  */
    CALL = 268,                    /* CALL  */
    RETURN = 269,                  /* RETURN  */
    INT = 270,                     /* INT  */
    AND = 271,                     /* AND  */
    OR = 272,                      /* OR  */
    EQ = 273,                      /* EQ  */
    NEQ = 274                      /* NEQ  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif
/* Token kinds.  */
#define YYEMPTY -2
#define YYEOF 0
#define YYerror 256
#define YYUNDEF 257
#define VAR 258
#define LABEL 259
#define IF 260
#define GOTO 261
#define STORE 262
#define LOAD 263
#define LOADADDR 264
#define MALLOC 265
#define FUN 266
#define END 267
#define CALL 268
#define RETURN 269
#define INT 270
#define AND 271
#define OR 272
#define EQ 273
#define NEQ 274

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef int YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_TIGGER2RISCV_TAB_HPP_INCLUDED  */
