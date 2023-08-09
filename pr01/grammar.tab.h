/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_GRAMMAR_TAB_H_INCLUDED
# define YY_YY_GRAMMAR_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    ID = 101,
    T_INTEGER = 201,
    T_REAL = 202,
    T_BOOLEAN = 203,
    T_CHARACTER = 204,
    T_STRING = 205,
    C_INTEGER = 301,
    C_REAL = 302,
    C_CHARACTER = 303,
    C_STRING = 304,
    C_TRUE = 305,
    C_FALSE = 306,
    NULL_PTR = 401,
    FOR = 402,
    WHILE = 403,
    IF = 404,
    THEN = 405,
    ELSE = 406,
    SWITCH = 407,
    CASE = 408,
    OTHERWISE = 409,
    TYPE = 410,
    FUNCTION = 411,
    L_PARENTHESIS = 501,
    R_PARENTHESIS = 502,
    L_BRACKET = 503,
    R_BRACKET = 504,
    L_BRACE = 505,
    R_BRACE = 506,
    SEMI_COLON = 507,
    COLON = 508,
    COMMA = 509,
    ARROW = 510,
    DOT = 511,
    ADD = 601,
    SUB_OR_NEG = 602,
    MUL = 603,
    DIV = 604,
    REM = 605,
    LESS_THAN = 606,
    EQUAL_TO = 607,
    ASSIGN = 608,
    INT2REAL = 609,
    REAL2INT = 610,
    IS_NULL = 611,
    NOT = 612,
    AND = 613,
    OR = 614,
    RESERVE = 615,
    RELEASE = 616,
    COMMENT = 700
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 21 "grammar.y"

 struct Basic *basic;
 struct ConstantValue *k;
 struct ExpressionTypeInfo *t;
 struct StatementInfo *stmt;
 struct SymbolTableEntry *symtabe;
 struct ParameterDec *Pd;
 struct SymbolTable *symtab;


#line 122 "grammar.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAMMAR_TAB_H_INCLUDED  */
