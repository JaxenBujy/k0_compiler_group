/* A Bison parser, made by GNU Bison 3.8.2.  */

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

#ifndef YY_YY_K0GRAM_TAB_H_INCLUDED
# define YY_YY_K0GRAM_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
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
    BREAK = 258,                   /* BREAK  */
    CONTINUE = 259,                /* CONTINUE  */
    DO = 260,                      /* DO  */
    ELSE = 261,                    /* ELSE  */
    K_FALSE = 262,                 /* K_FALSE  */
    FOR = 263,                     /* FOR  */
    FUN = 264,                     /* FUN  */
    IF = 265,                      /* IF  */
    IN = 266,                      /* IN  */
    K_NULL = 267,                  /* K_NULL  */
    RETURN = 268,                  /* RETURN  */
    K_TRUE = 269,                  /* K_TRUE  */
    VAL = 270,                     /* VAL  */
    VAR = 271,                     /* VAR  */
    WHEN = 272,                    /* WHEN  */
    WHILE = 273,                   /* WHILE  */
    IMPORT = 274,                  /* IMPORT  */
    OP_ERROR = 275,                /* OP_ERROR  */
    ILL_IDENT = 276,               /* ILL_IDENT  */
    UNCLOSED_COMMENT = 277,        /* UNCLOSED_COMMENT  */
    COULD_NOT_MATCH = 278,         /* COULD_NOT_MATCH  */
    KEYWORD_ERROR = 279,           /* KEYWORD_ERROR  */
    ASSIGN = 280,                  /* ASSIGN  */
    PLUS_ASSIGN = 281,             /* PLUS_ASSIGN  */
    MINUS_ASSIGN = 282,            /* MINUS_ASSIGN  */
    PLUS = 283,                    /* PLUS  */
    MINUS = 284,                   /* MINUS  */
    MUL = 285,                     /* MUL  */
    DIV = 286,                     /* DIV  */
    MOD = 287,                     /* MOD  */
    INC = 288,                     /* INC  */
    DEC = 289,                     /* DEC  */
    NEG = 290,                     /* NEG  */
    EQ = 291,                      /* EQ  */
    NEQ = 292,                     /* NEQ  */
    GT = 293,                      /* GT  */
    LT = 294,                      /* LT  */
    GTE = 295,                     /* GTE  */
    LTE = 296,                     /* LTE  */
    REQ = 297,                     /* REQ  */
    RNEQ = 298,                    /* RNEQ  */
    AND = 299,                     /* AND  */
    OR = 300,                      /* OR  */
    NOT = 301,                     /* NOT  */
    NN_ASSERT = 302,               /* NN_ASSERT  */
    LSQUARE = 303,                 /* LSQUARE  */
    DOT = 304,                     /* DOT  */
    SAFE_CALL = 305,               /* SAFE_CALL  */
    ELVIS = 306,                   /* ELVIS  */
    NULLABLE = 307,                /* NULLABLE  */
    RANGE_INCL = 308,              /* RANGE_INCL  */
    RANGE_EXCL = 309,              /* RANGE_EXCL  */
    TYPE_CAST = 310,               /* TYPE_CAST  */
    LPAREN = 311,                  /* LPAREN  */
    RPAREN = 312,                  /* RPAREN  */
    COMMA = 313,                   /* COMMA  */
    SEMICOLON = 314,               /* SEMICOLON  */
    LBRACE = 315,                  /* LBRACE  */
    RBRACE = 316,                  /* RBRACE  */
    COLON = 317,                   /* COLON  */
    RSQUARE = 318,                 /* RSQUARE  */
    INT = 319,                     /* INT  */
    REAL = 320,                    /* REAL  */
    STRING = 321,                  /* STRING  */
    MULTI_STRING = 322,            /* MULTI_STRING  */
    CHAR = 323,                    /* CHAR  */
    IDENT = 324,                   /* IDENT  */
    BYTE_TYPE = 325,               /* BYTE_TYPE  */
    SHORT_TYPE = 326,              /* SHORT_TYPE  */
    INT_TYPE = 327,                /* INT_TYPE  */
    LONG_TYPE = 328,               /* LONG_TYPE  */
    FLOAT_TYPE = 329,              /* FLOAT_TYPE  */
    DOUBLE_TYPE = 330,             /* DOUBLE_TYPE  */
    BOOLEAN_TYPE = 331,            /* BOOLEAN_TYPE  */
    STRING_TYPE = 332,             /* STRING_TYPE  */
    ARRAY = 333,                   /* ARRAY  */
    IFX = 334                      /* IFX  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 14 "k0gram.y"

    struct tree *treeptr;

#line 147 "k0gram.tab.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_K0GRAM_TAB_H_INCLUDED  */
