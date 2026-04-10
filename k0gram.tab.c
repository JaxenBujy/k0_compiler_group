/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "k0gram.y"

    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "symtab.h"
    extern int yylex();
    extern int yyerror(const char *s);
    struct tree *alctree(int prod_rule, char *symbol_name, int nkids, struct tree *kids[10], struct token *leaf);
    void print_node(struct tree *t);
    struct tree *root = NULL;
    int serial = 0;

#line 84 "k0gram.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "k0gram.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_BREAK = 3,                      /* BREAK  */
  YYSYMBOL_CONTINUE = 4,                   /* CONTINUE  */
  YYSYMBOL_DO = 5,                         /* DO  */
  YYSYMBOL_ELSE = 6,                       /* ELSE  */
  YYSYMBOL_K_FALSE = 7,                    /* K_FALSE  */
  YYSYMBOL_FOR = 8,                        /* FOR  */
  YYSYMBOL_FUN = 9,                        /* FUN  */
  YYSYMBOL_IF = 10,                        /* IF  */
  YYSYMBOL_IN = 11,                        /* IN  */
  YYSYMBOL_K_NULL = 12,                    /* K_NULL  */
  YYSYMBOL_RETURN = 13,                    /* RETURN  */
  YYSYMBOL_K_TRUE = 14,                    /* K_TRUE  */
  YYSYMBOL_VAL = 15,                       /* VAL  */
  YYSYMBOL_VAR = 16,                       /* VAR  */
  YYSYMBOL_WHEN = 17,                      /* WHEN  */
  YYSYMBOL_WHILE = 18,                     /* WHILE  */
  YYSYMBOL_IMPORT = 19,                    /* IMPORT  */
  YYSYMBOL_OP_ERROR = 20,                  /* OP_ERROR  */
  YYSYMBOL_ILL_IDENT = 21,                 /* ILL_IDENT  */
  YYSYMBOL_UNCLOSED_COMMENT = 22,          /* UNCLOSED_COMMENT  */
  YYSYMBOL_COULD_NOT_MATCH = 23,           /* COULD_NOT_MATCH  */
  YYSYMBOL_KEYWORD_ERROR = 24,             /* KEYWORD_ERROR  */
  YYSYMBOL_ASSIGN = 25,                    /* ASSIGN  */
  YYSYMBOL_PLUS_ASSIGN = 26,               /* PLUS_ASSIGN  */
  YYSYMBOL_MINUS_ASSIGN = 27,              /* MINUS_ASSIGN  */
  YYSYMBOL_PLUS = 28,                      /* PLUS  */
  YYSYMBOL_MINUS = 29,                     /* MINUS  */
  YYSYMBOL_MUL = 30,                       /* MUL  */
  YYSYMBOL_DIV = 31,                       /* DIV  */
  YYSYMBOL_MOD = 32,                       /* MOD  */
  YYSYMBOL_INC = 33,                       /* INC  */
  YYSYMBOL_DEC = 34,                       /* DEC  */
  YYSYMBOL_NEG = 35,                       /* NEG  */
  YYSYMBOL_EQ = 36,                        /* EQ  */
  YYSYMBOL_NEQ = 37,                       /* NEQ  */
  YYSYMBOL_GT = 38,                        /* GT  */
  YYSYMBOL_LT = 39,                        /* LT  */
  YYSYMBOL_GTE = 40,                       /* GTE  */
  YYSYMBOL_LTE = 41,                       /* LTE  */
  YYSYMBOL_REQ = 42,                       /* REQ  */
  YYSYMBOL_RNEQ = 43,                      /* RNEQ  */
  YYSYMBOL_AND = 44,                       /* AND  */
  YYSYMBOL_OR = 45,                        /* OR  */
  YYSYMBOL_NOT = 46,                       /* NOT  */
  YYSYMBOL_NN_ASSERT = 47,                 /* NN_ASSERT  */
  YYSYMBOL_LSQUARE = 48,                   /* LSQUARE  */
  YYSYMBOL_DOT = 49,                       /* DOT  */
  YYSYMBOL_SAFE_CALL = 50,                 /* SAFE_CALL  */
  YYSYMBOL_ELVIS = 51,                     /* ELVIS  */
  YYSYMBOL_NULLABLE = 52,                  /* NULLABLE  */
  YYSYMBOL_RANGE_INCL = 53,                /* RANGE_INCL  */
  YYSYMBOL_RANGE_EXCL = 54,                /* RANGE_EXCL  */
  YYSYMBOL_TYPE_CAST = 55,                 /* TYPE_CAST  */
  YYSYMBOL_LPAREN = 56,                    /* LPAREN  */
  YYSYMBOL_RPAREN = 57,                    /* RPAREN  */
  YYSYMBOL_COMMA = 58,                     /* COMMA  */
  YYSYMBOL_SEMICOLON = 59,                 /* SEMICOLON  */
  YYSYMBOL_LBRACE = 60,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 61,                    /* RBRACE  */
  YYSYMBOL_COLON = 62,                     /* COLON  */
  YYSYMBOL_RSQUARE = 63,                   /* RSQUARE  */
  YYSYMBOL_INT = 64,                       /* INT  */
  YYSYMBOL_REAL = 65,                      /* REAL  */
  YYSYMBOL_STRING = 66,                    /* STRING  */
  YYSYMBOL_MULTI_STRING = 67,              /* MULTI_STRING  */
  YYSYMBOL_CHAR = 68,                      /* CHAR  */
  YYSYMBOL_IDENT = 69,                     /* IDENT  */
  YYSYMBOL_BYTE_TYPE = 70,                 /* BYTE_TYPE  */
  YYSYMBOL_SHORT_TYPE = 71,                /* SHORT_TYPE  */
  YYSYMBOL_INT_TYPE = 72,                  /* INT_TYPE  */
  YYSYMBOL_LONG_TYPE = 73,                 /* LONG_TYPE  */
  YYSYMBOL_FLOAT_TYPE = 74,                /* FLOAT_TYPE  */
  YYSYMBOL_DOUBLE_TYPE = 75,               /* DOUBLE_TYPE  */
  YYSYMBOL_BOOLEAN_TYPE = 76,              /* BOOLEAN_TYPE  */
  YYSYMBOL_STRING_TYPE = 77,               /* STRING_TYPE  */
  YYSYMBOL_CHAR_TYPE = 78,                 /* CHAR_TYPE  */
  YYSYMBOL_NULL_TYPE = 79,                 /* NULL_TYPE  */
  YYSYMBOL_ARRAY = 80,                     /* ARRAY  */
  YYSYMBOL_IFX = 81,                       /* IFX  */
  YYSYMBOL_YYACCEPT = 82,                  /* $accept  */
  YYSYMBOL_program = 83,                   /* program  */
  YYSYMBOL_top_level_list = 84,            /* top_level_list  */
  YYSYMBOL_top_level_decl = 85,            /* top_level_decl  */
  YYSYMBOL_importHeader = 86,              /* importHeader  */
  YYSYMBOL_type = 87,                      /* type  */
  YYSYMBOL_val_var = 88,                   /* val_var  */
  YYSYMBOL_literal = 89,                   /* literal  */
  YYSYMBOL_bool_literal = 90,              /* bool_literal  */
  YYSYMBOL_global_var_decl = 91,           /* global_var_decl  */
  YYSYMBOL_global_var_init = 92,           /* global_var_init  */
  YYSYMBOL_fun_body_var_decl = 93,         /* fun_body_var_decl  */
  YYSYMBOL_fun_body_var_init = 94,         /* fun_body_var_init  */
  YYSYMBOL_function_var_decl = 95,         /* function_var_decl  */
  YYSYMBOL_function_decl = 96,             /* function_decl  */
  YYSYMBOL_parameter_list = 97,            /* parameter_list  */
  YYSYMBOL_function_body = 98,             /* function_body  */
  YYSYMBOL_fun_body_var_list = 99,         /* fun_body_var_list  */
  YYSYMBOL_statement_list = 100,           /* statement_list  */
  YYSYMBOL_statement = 101,                /* statement  */
  YYSYMBOL_non_control_statement = 102,    /* non_control_statement  */
  YYSYMBOL_expr = 103,                     /* expr  */
  YYSYMBOL_assignment_expr = 104,          /* assignment_expr  */
  YYSYMBOL_elvis_expr = 105,               /* elvis_expr  */
  YYSYMBOL_logical_or_expr = 106,          /* logical_or_expr  */
  YYSYMBOL_logical_and_expr = 107,         /* logical_and_expr  */
  YYSYMBOL_equality_expr = 108,            /* equality_expr  */
  YYSYMBOL_relational_expr = 109,          /* relational_expr  */
  YYSYMBOL_range_expr = 110,               /* range_expr  */
  YYSYMBOL_additive_expr = 111,            /* additive_expr  */
  YYSYMBOL_multiplicative_expr = 112,      /* multiplicative_expr  */
  YYSYMBOL_unary_expr = 113,               /* unary_expr  */
  YYSYMBOL_postfix_expr = 114,             /* postfix_expr  */
  YYSYMBOL_primary_expr = 115,             /* primary_expr  */
  YYSYMBOL_function_call = 116,            /* function_call  */
  YYSYMBOL_function_call_values_list = 117, /* function_call_values_list  */
  YYSYMBOL_functionCallVal = 118,          /* functionCallVal  */
  YYSYMBOL_block = 119,                    /* block  */
  YYSYMBOL_loop_statement = 120,           /* loop_statement  */
  YYSYMBOL_for_statement = 121,            /* for_statement  */
  YYSYMBOL_while_statement = 122,          /* while_statement  */
  YYSYMBOL_control_structure_body = 123,   /* control_structure_body  */
  YYSYMBOL_if_statement = 124              /* if_statement  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
# define YYCOPY_NEEDED 1
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  15
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   286

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  82
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  43
/* YYNRULES -- Number of rules.  */
#define YYNRULES  126
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  233

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   336


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   127,   127,   130,   131,   134,   135,   136,   137,   140,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   156,   157,   160,   161,   162,   163,   164,   167,   168,
     174,   175,   176,   177,   182,   187,   188,   189,   190,   195,
     198,   199,   206,   207,   208,   211,   212,   213,   218,   221,
     222,   223,   226,   227,   230,   231,   232,   233,   236,   239,
     242,   243,   244,   245,   248,   249,   252,   253,   256,   257,
     260,   261,   262,   263,   264,   267,   268,   269,   270,   271,
     274,   275,   276,   279,   280,   281,   284,   285,   286,   287,
     290,   291,   292,   293,   294,   295,   298,   299,   300,   301,
     302,   303,   304,   307,   308,   309,   310,   311,   314,   317,
     318,   319,   322,   323,   324,   327,   330,   331,   334,   335,
     336,   337,   340,   343,   344,   347,   348
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "BREAK", "CONTINUE", "DO",
  "ELSE", "K_FALSE", "FOR", "FUN", "IF", "IN", "K_NULL", "RETURN",
  "K_TRUE", "VAL", "VAR", "WHEN", "WHILE", "IMPORT", "OP_ERROR",
  "ILL_IDENT", "UNCLOSED_COMMENT", "COULD_NOT_MATCH", "KEYWORD_ERROR",
  "ASSIGN", "PLUS_ASSIGN", "MINUS_ASSIGN", "PLUS", "MINUS", "MUL", "DIV",
  "MOD", "INC", "DEC", "NEG", "EQ", "NEQ", "GT", "LT", "GTE", "LTE", "REQ",
  "RNEQ", "AND", "OR", "NOT", "NN_ASSERT", "LSQUARE", "DOT", "SAFE_CALL",
  "ELVIS", "NULLABLE", "RANGE_INCL", "RANGE_EXCL", "TYPE_CAST", "LPAREN",
  "RPAREN", "COMMA", "SEMICOLON", "LBRACE", "RBRACE", "COLON", "RSQUARE",
  "INT", "REAL", "STRING", "MULTI_STRING", "CHAR", "IDENT", "BYTE_TYPE",
  "SHORT_TYPE", "INT_TYPE", "LONG_TYPE", "FLOAT_TYPE", "DOUBLE_TYPE",
  "BOOLEAN_TYPE", "STRING_TYPE", "CHAR_TYPE", "NULL_TYPE", "ARRAY", "IFX",
  "$accept", "program", "top_level_list", "top_level_decl", "importHeader",
  "type", "val_var", "literal", "bool_literal", "global_var_decl",
  "global_var_init", "fun_body_var_decl", "fun_body_var_init",
  "function_var_decl", "function_decl", "parameter_list", "function_body",
  "fun_body_var_list", "statement_list", "statement",
  "non_control_statement", "expr", "assignment_expr", "elvis_expr",
  "logical_or_expr", "logical_and_expr", "equality_expr",
  "relational_expr", "range_expr", "additive_expr", "multiplicative_expr",
  "unary_expr", "postfix_expr", "primary_expr", "function_call",
  "function_call_values_list", "functionCallVal", "block",
  "loop_statement", "for_statement", "while_statement",
  "control_structure_body", "if_statement", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-184)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      94,   -65,  -184,  -184,   -38,    39,    94,  -184,  -184,   -36,
    -184,  -184,  -184,    -8,  -184,  -184,  -184,   -11,     4,   161,
     190,    16,  -184,    47,  -184,  -184,   189,   189,   189,   189,
     189,   161,  -184,  -184,  -184,  -184,  -184,    -1,  -184,  -184,
      21,  -184,  -184,   -16,    46,    86,   142,    62,    90,    31,
    -184,    32,  -184,  -184,  -184,  -184,  -184,  -184,  -184,  -184,
    -184,  -184,  -184,  -184,    44,    -6,   190,   -32,     4,    52,
    -184,  -184,  -184,  -184,  -184,    63,   161,   161,   161,    33,
    -184,   189,   189,   189,   189,   189,   189,   189,   189,   189,
     189,   189,   189,   189,   189,   189,   189,   189,   189,  -184,
    -184,  -184,    56,    66,  -184,   190,   161,    -5,  -184,    75,
    -184,   190,  -184,  -184,  -184,  -184,  -184,  -184,   105,  -184,
    -184,    73,  -184,    46,  -184,    86,   142,   142,   142,   142,
      62,    62,    62,    62,    90,    90,    31,    31,  -184,  -184,
    -184,  -184,  -184,   103,    89,   161,  -184,  -184,   124,    54,
      97,  -184,    33,  -184,  -184,   106,    87,  -184,  -184,     3,
      72,  -184,   108,  -184,  -184,   -10,   104,   111,   161,   120,
    -184,  -184,  -184,   130,  -184,  -184,  -184,  -184,  -184,    93,
     161,   190,   128,   161,   139,   161,  -184,    52,  -184,   140,
      -2,   191,   147,  -184,   148,  -184,   161,    -3,  -184,   172,
      78,    78,   149,   161,  -184,   162,   166,  -184,  -184,  -184,
     214,  -184,  -184,   165,   182,   207,   145,    78,  -184,   164,
     185,   186,   195,  -184,  -184,    78,    78,    78,    78,  -184,
    -184,  -184,  -184
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    21,    22,     0,     0,     2,     4,     6,     0,
       7,     8,     5,     0,     9,     1,     3,     0,    47,     0,
       0,     0,    45,     0,    29,    28,     0,     0,     0,     0,
       0,     0,    23,    24,    25,    26,    27,   104,   105,   107,
       0,    59,    60,    64,    66,    68,    70,    75,    80,    83,
      86,    90,    96,   103,    10,    11,    12,    13,    14,    15,
      16,    17,    19,    18,     0,     0,     0,     0,     0,   104,
      91,    93,    94,    92,    95,     0,     0,     0,     0,   111,
      34,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
      98,    99,     0,     0,   100,     0,     0,     0,    30,    40,
      51,     0,    44,    46,   106,    61,    62,    63,     0,   113,
     112,     0,   109,    67,    65,    69,    71,    72,    73,    74,
      77,    76,    79,    78,    81,    82,    84,    85,    87,    88,
      89,   101,   102,     0,     0,     0,    32,    41,    53,     0,
       0,   108,     0,    20,    31,     0,     0,    49,    50,     0,
       0,    42,     0,   110,    33,     0,     0,     0,     0,     0,
      48,    52,    54,     0,    55,   116,   117,    56,    43,     0,
       0,     0,     0,     0,     0,     0,    58,     0,   114,     0,
       0,     0,     0,    57,     0,    39,     0,     0,    35,     0,
       0,     0,     0,     0,    37,     0,     0,    53,   124,   123,
     125,   122,    36,     0,     0,     0,     0,     0,    38,     0,
       0,     0,     0,   115,   126,     0,     0,     0,     0,   118,
     120,   119,   121
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -184,  -184,  -184,   253,  -184,   -64,   129,   -78,  -184,  -184,
    -184,  -184,  -184,   210,  -184,  -184,  -122,  -184,    74,  -156,
    -184,   -19,    17,   197,  -184,   199,   200,   101,   143,    58,
      98,   -21,  -184,  -184,   107,  -184,   132,  -184,  -184,  -184,
    -184,  -183,  -184
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     5,     6,     7,     8,    65,     9,    38,    39,    10,
      11,   157,   158,    22,    12,    23,   112,   148,   159,   208,
     172,   173,    41,    42,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,   121,   122,   209,   174,   175,
     176,   210,   177
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      40,   120,   109,   171,    13,    70,    71,    72,    73,    74,
      24,   166,    75,   167,    19,   180,   168,    25,   211,   106,
     145,   169,   203,   196,    76,    77,    78,   161,   110,    81,
     111,    14,    26,    17,   224,    82,    27,    28,   178,    15,
     150,   143,   229,   230,   231,   232,   107,   149,    18,    29,
     197,    20,   181,   108,   146,    79,   204,   198,    30,    31,
     171,    96,    97,    98,   170,    99,   100,    32,    33,    34,
      35,    36,    37,    21,   120,   138,   139,   140,    66,   101,
      80,   102,   103,   105,   104,    24,   166,   144,   167,   118,
      83,   168,    25,   115,   116,   117,   169,    32,    33,    34,
      35,    36,   119,     1,    67,    68,   160,    26,    79,     2,
       3,    27,    28,     4,   110,    92,    93,   190,    94,    95,
     114,   206,    84,    85,    29,   141,   155,   147,    86,    87,
     151,   152,   110,    30,    31,   142,   220,   222,   207,     2,
       3,   153,    32,    33,    34,    35,    36,    37,   154,   184,
     134,   135,    24,   166,   162,   167,   165,   179,   168,    25,
     182,   189,   187,   169,   192,   164,   194,   183,    24,    32,
      33,    34,    35,    36,    26,    25,   185,   202,    27,    28,
      88,    89,    90,    91,   213,   126,   127,   128,   129,   186,
      26,    29,   136,   137,    27,    28,    24,   191,   193,   195,
      30,    31,   199,    25,   200,   201,   223,    29,   212,    32,
      33,    34,    35,    36,    37,   214,    30,    31,    26,   215,
     217,   225,    27,    28,   218,    32,    33,    34,    35,    36,
      37,   130,   131,   132,   133,    29,    32,    33,    34,    35,
      36,   205,   226,   227,    30,    31,    32,    33,    34,    35,
      36,   219,   228,    32,    33,    34,    35,    36,    69,    16,
      54,    55,    56,    57,    58,    59,    60,    61,    62,    63,
      64,    32,    33,    34,    35,    36,   221,   156,   113,   124,
     123,   216,     0,   125,   163,     0,   188
};

static const yytype_int16 yycheck[] =
{
      19,    79,    66,   159,    69,    26,    27,    28,    29,    30,
       7,     8,    31,    10,    25,    25,    13,    14,   201,    25,
      25,    18,    25,    25,    25,    26,    27,   149,    60,    45,
      62,    69,    29,    69,   217,    51,    33,    34,   160,     0,
     118,   105,   225,   226,   227,   228,    52,   111,    56,    46,
      52,    62,    62,    59,    59,    56,    59,    59,    55,    56,
     216,    30,    31,    32,    61,    33,    34,    64,    65,    66,
      67,    68,    69,    69,   152,    96,    97,    98,    62,    47,
      59,    49,    50,    39,    52,     7,     8,   106,    10,    56,
      44,    13,    14,    76,    77,    78,    18,    64,    65,    66,
      67,    68,    69,     9,    57,    58,    52,    29,    56,    15,
      16,    33,    34,    19,    60,    53,    54,   181,    28,    29,
      57,   199,    36,    37,    46,    69,   145,    52,    42,    43,
      57,    58,    60,    55,    56,    69,   214,   215,    60,    15,
      16,    38,    64,    65,    66,    67,    68,    69,    59,   168,
      92,    93,     7,     8,    57,    10,    69,    49,    13,    14,
      56,   180,    69,    18,   183,    59,   185,    56,     7,    64,
      65,    66,    67,    68,    29,    14,    56,   196,    33,    34,
      38,    39,    40,    41,   203,    84,    85,    86,    87,    59,
      29,    46,    94,    95,    33,    34,     7,    69,    59,    59,
      55,    56,    11,    14,    57,    57,    61,    46,    59,    64,
      65,    66,    67,    68,    69,    53,    55,    56,    29,    53,
       6,    57,    33,    34,    59,    64,    65,    66,    67,    68,
      69,    88,    89,    90,    91,    46,    64,    65,    66,    67,
      68,    69,    57,    57,    55,    56,    64,    65,    66,    67,
      68,    69,    57,    64,    65,    66,    67,    68,    69,     6,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    64,    65,    66,    67,    68,    69,   148,    68,    82,
      81,   207,    -1,    83,   152,    -1,   179
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     9,    15,    16,    19,    83,    84,    85,    86,    88,
      91,    92,    96,    69,    69,     0,    85,    69,    56,    25,
      62,    69,    95,    97,     7,    14,    29,    33,    34,    46,
      55,    56,    64,    65,    66,    67,    68,    69,    89,    90,
     103,   104,   105,   106,   107,   108,   109,   110,   111,   112,
     113,   114,   115,   116,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    87,    62,    57,    58,    69,
     113,   113,   113,   113,   113,   103,    25,    26,    27,    56,
      59,    45,    51,    44,    36,    37,    42,    43,    38,    39,
      40,    41,    53,    54,    28,    29,    30,    31,    32,    33,
      34,    47,    49,    50,    52,    39,    25,    52,    59,    87,
      60,    62,    98,    95,    57,   104,   104,   104,    56,    69,
      89,   117,   118,   107,   105,   108,   109,   109,   109,   109,
     110,   110,   110,   110,   111,   111,   112,   112,   113,   113,
     113,    69,    69,    87,   103,    25,    59,    52,    99,    87,
      89,    57,    58,    38,    59,   103,    88,    93,    94,   100,
      52,    98,    57,   118,    59,    69,     8,    10,    13,    18,
      61,   101,   102,   103,   120,   121,   122,   124,    98,    49,
      25,    62,    56,    56,   103,    56,    59,    69,   116,   103,
      87,    69,   103,    59,   103,    59,    25,    52,    59,    11,
      57,    57,   103,    25,    59,    69,    89,    60,   101,   119,
     123,   123,    59,   103,    53,    53,   100,     6,    59,    69,
      89,    69,    89,    61,   123,    57,    57,    57,    57,   123,
     123,   123,   123
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    82,    83,    84,    84,    85,    85,    85,    85,    86,
      87,    87,    87,    87,    87,    87,    87,    87,    87,    87,
      87,    88,    88,    89,    89,    89,    89,    89,    90,    90,
      91,    91,    91,    91,    92,    93,    93,    93,    93,    94,
      95,    95,    96,    96,    96,    97,    97,    97,    98,    99,
      99,    99,   100,   100,   101,   101,   101,   101,   102,   103,
     104,   104,   104,   104,   105,   105,   106,   106,   107,   107,
     108,   108,   108,   108,   108,   109,   109,   109,   109,   109,
     110,   110,   110,   111,   111,   111,   112,   112,   112,   112,
     113,   113,   113,   113,   113,   113,   114,   114,   114,   114,
     114,   114,   114,   115,   115,   115,   115,   115,   116,   117,
     117,   117,   118,   118,   118,   119,   120,   120,   121,   121,
     121,   121,   122,   123,   123,   124,   124
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     2,     1,     1,     1,     1,     1,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       5,     7,     6,     8,     5,     5,     7,     6,     8,     5,
       3,     4,     8,     9,     6,     1,     3,     0,     4,     2,
       2,     0,     2,     0,     1,     1,     1,     3,     2,     1,
       1,     3,     3,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     3,     3,
       1,     3,     3,     1,     3,     3,     1,     3,     3,     3,
       1,     2,     2,     2,     2,     2,     1,     2,     2,     2,
       2,     3,     3,     1,     1,     1,     3,     1,     4,     1,
       3,     0,     1,     1,     5,     3,     1,     1,     9,     9,
       9,     9,     5,     1,     1,     5,     7
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        YY_LAC_DISCARD ("YYBACKUP");                              \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Given a state stack such that *YYBOTTOM is its bottom, such that
   *YYTOP is either its top or is YYTOP_EMPTY to indicate an empty
   stack, and such that *YYCAPACITY is the maximum number of elements it
   can hold without a reallocation, make sure there is enough room to
   store YYADD more elements.  If not, allocate a new stack using
   YYSTACK_ALLOC, copy the existing elements, and adjust *YYBOTTOM,
   *YYTOP, and *YYCAPACITY to reflect the new capacity and memory
   location.  If *YYBOTTOM != YYBOTTOM_NO_FREE, then free the old stack
   using YYSTACK_FREE.  Return 0 if successful or if no reallocation is
   required.  Return YYENOMEM if memory is exhausted.  */
static int
yy_lac_stack_realloc (YYPTRDIFF_T *yycapacity, YYPTRDIFF_T yyadd,
#if YYDEBUG
                      char const *yydebug_prefix,
                      char const *yydebug_suffix,
#endif
                      yy_state_t **yybottom,
                      yy_state_t *yybottom_no_free,
                      yy_state_t **yytop, yy_state_t *yytop_empty)
{
  YYPTRDIFF_T yysize_old =
    *yytop == yytop_empty ? 0 : *yytop - *yybottom + 1;
  YYPTRDIFF_T yysize_new = yysize_old + yyadd;
  if (*yycapacity < yysize_new)
    {
      YYPTRDIFF_T yyalloc = 2 * yysize_new;
      yy_state_t *yybottom_new;
      /* Use YYMAXDEPTH for maximum stack size given that the stack
         should never need to grow larger than the main state stack
         needs to grow without LAC.  */
      if (YYMAXDEPTH < yysize_new)
        {
          YYDPRINTF ((stderr, "%smax size exceeded%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (YYMAXDEPTH < yyalloc)
        yyalloc = YYMAXDEPTH;
      yybottom_new =
        YY_CAST (yy_state_t *,
                 YYSTACK_ALLOC (YY_CAST (YYSIZE_T,
                                         yyalloc * YYSIZEOF (*yybottom_new))));
      if (!yybottom_new)
        {
          YYDPRINTF ((stderr, "%srealloc failed%s", yydebug_prefix,
                      yydebug_suffix));
          return YYENOMEM;
        }
      if (*yytop != yytop_empty)
        {
          YYCOPY (yybottom_new, *yybottom, yysize_old);
          *yytop = yybottom_new + (yysize_old - 1);
        }
      if (*yybottom != yybottom_no_free)
        YYSTACK_FREE (*yybottom);
      *yybottom = yybottom_new;
      *yycapacity = yyalloc;
    }
  return 0;
}

/* Establish the initial context for the current lookahead if no initial
   context is currently established.

   We define a context as a snapshot of the parser stacks.  We define
   the initial context for a lookahead as the context in which the
   parser initially examines that lookahead in order to select a
   syntactic action.  Thus, if the lookahead eventually proves
   syntactically unacceptable (possibly in a later context reached via a
   series of reductions), the initial context can be used to determine
   the exact set of tokens that would be syntactically acceptable in the
   lookahead's place.  Moreover, it is the context after which any
   further semantic actions would be erroneous because they would be
   determined by a syntactically unacceptable token.

   YY_LAC_ESTABLISH should be invoked when a reduction is about to be
   performed in an inconsistent state (which, for the purposes of LAC,
   includes consistent states that don't know they're consistent because
   their default reductions have been disabled).  Iff there is a
   lookahead token, it should also be invoked before reporting a syntax
   error.  This latter case is for the sake of the debugging output.

   For parse.lac=full, the implementation of YY_LAC_ESTABLISH is as
   follows.  If no initial context is currently established for the
   current lookahead, then check if that lookahead can eventually be
   shifted if syntactic actions continue from the current context.
   Report a syntax error if it cannot.  */
#define YY_LAC_ESTABLISH                                                \
do {                                                                    \
  if (!yy_lac_established)                                              \
    {                                                                   \
      YYDPRINTF ((stderr,                                               \
                  "LAC: initial context established for %s\n",          \
                  yysymbol_name (yytoken)));                            \
      yy_lac_established = 1;                                           \
      switch (yy_lac (yyesa, &yyes, &yyes_capacity, yyssp, yytoken))    \
        {                                                               \
        case YYENOMEM:                                                  \
          YYNOMEM;                                                      \
        case 1:                                                         \
          goto yyerrlab;                                                \
        }                                                               \
    }                                                                   \
} while (0)

/* Discard any previous initial lookahead context because of Event,
   which may be a lookahead change or an invalidation of the currently
   established initial context for the current lookahead.

   The most common example of a lookahead change is a shift.  An example
   of both cases is syntax error recovery.  That is, a syntax error
   occurs when the lookahead is syntactically erroneous for the
   currently established initial context, so error recovery manipulates
   the parser stacks to try to find a new initial context in which the
   current lookahead is syntactically acceptable.  If it fails to find
   such a context, it discards the lookahead.  */
#if YYDEBUG
# define YY_LAC_DISCARD(Event)                                           \
do {                                                                     \
  if (yy_lac_established)                                                \
    {                                                                    \
      YYDPRINTF ((stderr, "LAC: initial context discarded due to "       \
                  Event "\n"));                                          \
      yy_lac_established = 0;                                            \
    }                                                                    \
} while (0)
#else
# define YY_LAC_DISCARD(Event) yy_lac_established = 0
#endif

/* Given the stack whose top is *YYSSP, return 0 iff YYTOKEN can
   eventually (after perhaps some reductions) be shifted, return 1 if
   not, or return YYENOMEM if memory is exhausted.  As preconditions and
   postconditions: *YYES_CAPACITY is the allocated size of the array to
   which *YYES points, and either *YYES = YYESA or *YYES points to an
   array allocated with YYSTACK_ALLOC.  yy_lac may overwrite the
   contents of either array, alter *YYES and *YYES_CAPACITY, and free
   any old *YYES other than YYESA.  */
static int
yy_lac (yy_state_t *yyesa, yy_state_t **yyes,
        YYPTRDIFF_T *yyes_capacity, yy_state_t *yyssp, yysymbol_kind_t yytoken)
{
  yy_state_t *yyes_prev = yyssp;
  yy_state_t *yyesp = yyes_prev;
  /* Reduce until we encounter a shift and thereby accept the token.  */
  YYDPRINTF ((stderr, "LAC: checking lookahead %s:", yysymbol_name (yytoken)));
  if (yytoken == YYSYMBOL_YYUNDEF)
    {
      YYDPRINTF ((stderr, " Always Err\n"));
      return 1;
    }
  while (1)
    {
      int yyrule = yypact[+*yyesp];
      if (yypact_value_is_default (yyrule)
          || (yyrule += yytoken) < 0 || YYLAST < yyrule
          || yycheck[yyrule] != yytoken)
        {
          /* Use the default action.  */
          yyrule = yydefact[+*yyesp];
          if (yyrule == 0)
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
        }
      else
        {
          /* Use the action from yytable.  */
          yyrule = yytable[yyrule];
          if (yytable_value_is_error (yyrule))
            {
              YYDPRINTF ((stderr, " Err\n"));
              return 1;
            }
          if (0 < yyrule)
            {
              YYDPRINTF ((stderr, " S%d\n", yyrule));
              return 0;
            }
          yyrule = -yyrule;
        }
      /* By now we know we have to simulate a reduce.  */
      YYDPRINTF ((stderr, " R%d", yyrule - 1));
      {
        /* Pop the corresponding number of values from the stack.  */
        YYPTRDIFF_T yylen = yyr2[yyrule];
        /* First pop from the LAC stack as many tokens as possible.  */
        if (yyesp != yyes_prev)
          {
            YYPTRDIFF_T yysize = yyesp - *yyes + 1;
            if (yylen < yysize)
              {
                yyesp -= yylen;
                yylen = 0;
              }
            else
              {
                yyesp = yyes_prev;
                yylen -= yysize;
              }
          }
        /* Only afterwards look at the main stack.  */
        if (yylen)
          yyesp = yyes_prev -= yylen;
      }
      /* Push the resulting state of the reduction.  */
      {
        yy_state_fast_t yystate;
        {
          const int yylhs = yyr1[yyrule] - YYNTOKENS;
          const int yyi = yypgoto[yylhs] + *yyesp;
          yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyesp
                     ? yytable[yyi]
                     : yydefgoto[yylhs]);
        }
        if (yyesp == yyes_prev)
          {
            yyesp = *yyes;
            YY_IGNORE_USELESS_CAST_BEGIN
            *yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        else
          {
            if (yy_lac_stack_realloc (yyes_capacity, 1,
#if YYDEBUG
                                      " (", ")",
#endif
                                      yyes, yyesa, &yyesp, yyes_prev))
              {
                YYDPRINTF ((stderr, "\n"));
                return YYENOMEM;
              }
            YY_IGNORE_USELESS_CAST_BEGIN
            *++yyesp = YY_CAST (yy_state_t, yystate);
            YY_IGNORE_USELESS_CAST_END
          }
        YYDPRINTF ((stderr, " G%d", yystate));
      }
    }
}

/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yy_state_t *yyesa;
  yy_state_t **yyes;
  YYPTRDIFF_T *yyes_capacity;
  yysymbol_kind_t yytoken;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;

  int yyx;
  for (yyx = 0; yyx < YYNTOKENS; ++yyx)
    {
      yysymbol_kind_t yysym = YY_CAST (yysymbol_kind_t, yyx);
      if (yysym != YYSYMBOL_YYerror && yysym != YYSYMBOL_YYUNDEF)
        switch (yy_lac (yyctx->yyesa, yyctx->yyes, yyctx->yyes_capacity, yyctx->yyssp, yysym))
          {
          case YYENOMEM:
            return YYENOMEM;
          case 1:
            continue;
          default:
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = yysym;
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
       In the first two cases, it might appear that the current syntax
       error should have been detected in the previous state when yy_lac
       was invoked.  However, at that time, there might have been a
       different syntax error that discarded a different initial context
       during error recovery, leaving behind the current lookahead.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      YYDPRINTF ((stderr, "Constructing syntax error message\n"));
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else if (yyn == 0)
        YYDPRINTF ((stderr, "No expected tokens.\n"));
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.  In order to see if a particular token T is a
   valid looakhead, invoke yy_lac (YYESA, YYES, YYES_CAPACITY, YYSSP, T).

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store or if
   yy_lac returned YYENOMEM.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    yy_state_t yyesa[20];
    yy_state_t *yyes = yyesa;
    YYPTRDIFF_T yyes_capacity = 20 < YYMAXDEPTH ? 20 : YYMAXDEPTH;

  /* Whether LAC context is established.  A Boolean.  */
  int yy_lac_established = 0;
  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    {
      YY_LAC_ESTABLISH;
      goto yydefault;
    }
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      YY_LAC_ESTABLISH;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  YY_LAC_DISCARD ("shift");
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  {
    int yychar_backup = yychar;
    switch (yyn)
      {
  case 2: /* program: top_level_list  */
#line 127 "k0gram.y"
                     {root = (yyval.treeptr); }
#line 1835 "k0gram.tab.c"
    break;

  case 3: /* top_level_list: top_level_list top_level_decl  */
#line 130 "k0gram.y"
                                    {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TOP_LEVEL_LIST_RECUR, "top_level_list", 2, kids, NULL); }
#line 1841 "k0gram.tab.c"
    break;

  case 4: /* top_level_list: top_level_decl  */
#line 131 "k0gram.y"
                     {struct tree *kids[10] = {(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TOP_LEVEL_LIST_SINGLE, "top_level_list", 1, kids, NULL); }
#line 1847 "k0gram.tab.c"
    break;

  case 5: /* top_level_decl: function_decl  */
#line 134 "k0gram.y"
                    {struct tree *kids[10] = {(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TOP_LEVEL_DECL_FUNCTION, "top_level_decl", 1, kids, NULL); }
#line 1853 "k0gram.tab.c"
    break;

  case 6: /* top_level_decl: importHeader  */
#line 135 "k0gram.y"
                   {struct tree *kids[10] = {(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TOP_LEVEL_DECL_IMPORT, "top_level_decl", 1, kids, NULL); }
#line 1859 "k0gram.tab.c"
    break;

  case 7: /* top_level_decl: global_var_decl  */
#line 136 "k0gram.y"
                      {struct tree *kids[10] = {(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TOP_LEVEL_DECL_GLOBAL_VAR_DECL, "top_level_decl", 1, kids, NULL); }
#line 1865 "k0gram.tab.c"
    break;

  case 8: /* top_level_decl: global_var_init  */
#line 137 "k0gram.y"
                      {struct tree *kids[10] = {(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TOP_LEVEL_DECL_GLOBAL_VAR_INIT, "top_level_decl", 1, kids, NULL); }
#line 1871 "k0gram.tab.c"
    break;

  case 9: /* importHeader: IMPORT IDENT  */
#line 140 "k0gram.y"
                   {struct tree *kids[10] = {(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_IMPORT_HEADER, "importHeader", 2, kids, NULL); }
#line 1877 "k0gram.tab.c"
    break;

  case 20: /* type: ARRAY LT type GT  */
#line 153 "k0gram.y"
                       {struct tree *kids[10] = {(yyvsp[-3].treeptr), (yyvsp[-2].treeptr), (yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_TYPE_ARRAY, "type", 4, kids, NULL); }
#line 1883 "k0gram.tab.c"
    break;

  case 30: /* global_var_decl: val_var IDENT COLON type SEMICOLON  */
#line 174 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_GLOBAL_VAR_DECL, "global_var_decl", 5, kids, NULL); }
#line 1889 "k0gram.tab.c"
    break;

  case 31: /* global_var_decl: val_var IDENT COLON type ASSIGN expr SEMICOLON  */
#line 175 "k0gram.y"
                                                     {struct tree *kids[10] = {(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_GLOBAL_VAR_DECL_ASSIGN, "global_var_decl", 7, kids, NULL); }
#line 1895 "k0gram.tab.c"
    break;

  case 32: /* global_var_decl: val_var IDENT COLON type NULLABLE SEMICOLON  */
#line 176 "k0gram.y"
                                                  {struct tree *kids[10] = {(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_GLOBAL_VAR_DECL_NULLABLE, "global_var_decl", 6, kids, NULL); }
#line 1901 "k0gram.tab.c"
    break;

  case 33: /* global_var_decl: val_var IDENT COLON type NULLABLE ASSIGN expr SEMICOLON  */
#line 177 "k0gram.y"
                                                              {struct tree *kids[10] = {(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_GLOBAL_VAR_DECL_ASSIGN_NULLABLE, "global_var_decl", 8, kids, NULL); }
#line 1907 "k0gram.tab.c"
    break;

  case 34: /* global_var_init: val_var IDENT ASSIGN expr SEMICOLON  */
#line 182 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_GLOBAL_VAR_INIT, "global_var_init", 5, kids, NULL); }
#line 1913 "k0gram.tab.c"
    break;

  case 35: /* fun_body_var_decl: val_var IDENT COLON type SEMICOLON  */
#line 187 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_DECL, "fun_body_var_decl", 5, kids, NULL); }
#line 1919 "k0gram.tab.c"
    break;

  case 36: /* fun_body_var_decl: val_var IDENT COLON type ASSIGN expr SEMICOLON  */
#line 188 "k0gram.y"
                                                     {struct tree *kids[10] = {(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_DECL_ASSIGN, "fun_body_var_decl", 7, kids, NULL); }
#line 1925 "k0gram.tab.c"
    break;

  case 37: /* fun_body_var_decl: val_var IDENT COLON type NULLABLE SEMICOLON  */
#line 189 "k0gram.y"
                                                  {struct tree *kids[10] = {(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_DECL_NULLABLE, "fun_body_var_decl", 6, kids, NULL); }
#line 1931 "k0gram.tab.c"
    break;

  case 38: /* fun_body_var_decl: val_var IDENT COLON type NULLABLE ASSIGN expr SEMICOLON  */
#line 190 "k0gram.y"
                                                              {struct tree *kids[10] = {(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_DECL_ASSIGN_NULLABLE, "fun_body_var_decl", 8, kids, NULL); }
#line 1937 "k0gram.tab.c"
    break;

  case 39: /* fun_body_var_init: val_var IDENT ASSIGN expr SEMICOLON  */
#line 195 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_INIT, "fun_body_var_init", 5, kids, NULL); }
#line 1943 "k0gram.tab.c"
    break;

  case 40: /* function_var_decl: IDENT COLON type  */
#line 198 "k0gram.y"
                       {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_VAR_DECL, "function_var_decl", 3, kids, NULL); }
#line 1949 "k0gram.tab.c"
    break;

  case 41: /* function_var_decl: IDENT COLON type NULLABLE  */
#line 199 "k0gram.y"
                                {struct tree *kids[10] = {(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_VAR_DECL_NULLABLE, "function_var_decl", 4, kids, NULL); }
#line 1955 "k0gram.tab.c"
    break;

  case 42: /* function_decl: FUN IDENT LPAREN parameter_list RPAREN COLON type function_body  */
#line 206 "k0gram.y"
                                                                      {struct tree *kids[10] = {(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_DECL_TYPED, "function_decl", 8, kids, NULL); }
#line 1961 "k0gram.tab.c"
    break;

  case 43: /* function_decl: FUN IDENT LPAREN parameter_list RPAREN COLON type NULLABLE function_body  */
#line 207 "k0gram.y"
                                                                               {struct tree *kids[10] = {(yyvsp[-8].treeptr),(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_DECL_TYPED_NULLABLE, "function_decl", 9, kids, NULL); }
#line 1967 "k0gram.tab.c"
    break;

  case 44: /* function_decl: FUN IDENT LPAREN parameter_list RPAREN function_body  */
#line 208 "k0gram.y"
                                                           {struct tree *kids[10] = {(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_DECL_UNTYPED, "function_decl", 6, kids, NULL); }
#line 1973 "k0gram.tab.c"
    break;

  case 46: /* parameter_list: parameter_list COMMA function_var_decl  */
#line 212 "k0gram.y"
                                             {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_PARAMETER_RECUR, "parameter_list", 3, kids, NULL); }
#line 1979 "k0gram.tab.c"
    break;

  case 47: /* parameter_list: %empty  */
#line 213 "k0gram.y"
      {(yyval.treeptr) = NULL; }
#line 1985 "k0gram.tab.c"
    break;

  case 48: /* function_body: LBRACE fun_body_var_list statement_list RBRACE  */
#line 218 "k0gram.y"
                                                     {struct tree *kids[10] = {(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_BODY, "function_body", 4, kids, NULL); }
#line 1991 "k0gram.tab.c"
    break;

  case 49: /* fun_body_var_list: fun_body_var_list fun_body_var_decl  */
#line 221 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_LIST_DECL, "fun_body_var_list", 2, kids, NULL); }
#line 1997 "k0gram.tab.c"
    break;

  case 50: /* fun_body_var_list: fun_body_var_list fun_body_var_init  */
#line 222 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUN_BODY_VAR_LIST_INIT, "fun_body_var_list", 2, kids, NULL); }
#line 2003 "k0gram.tab.c"
    break;

  case 51: /* fun_body_var_list: %empty  */
#line 223 "k0gram.y"
      {(yyval.treeptr) = NULL; }
#line 2009 "k0gram.tab.c"
    break;

  case 52: /* statement_list: statement_list statement  */
#line 226 "k0gram.y"
                               {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_STATEMENT_LIST_RECUR, "statement_list", 2, kids, NULL); }
#line 2015 "k0gram.tab.c"
    break;

  case 53: /* statement_list: %empty  */
#line 227 "k0gram.y"
      {(yyval.treeptr) = NULL; }
#line 2021 "k0gram.tab.c"
    break;

  case 57: /* statement: RETURN expr SEMICOLON  */
#line 233 "k0gram.y"
                            {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_STATEMENT_RETURN, "statement", 3, kids, NULL); }
#line 2027 "k0gram.tab.c"
    break;

  case 58: /* non_control_statement: expr SEMICOLON  */
#line 236 "k0gram.y"
                     {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_NON_CONTROL_STATEMENT, "non_control_statement", 2, kids, NULL); }
#line 2033 "k0gram.tab.c"
    break;

  case 61: /* assignment_expr: IDENT ASSIGN assignment_expr  */
#line 243 "k0gram.y"
                                   {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_ASSIGNMENT_ASSIGN, "assignment_expr", 3, kids, NULL); }
#line 2039 "k0gram.tab.c"
    break;

  case 62: /* assignment_expr: IDENT PLUS_ASSIGN assignment_expr  */
#line 244 "k0gram.y"
                                        {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_ASSIGNMENT_PLUS, "assignment_expr", 3, kids, NULL); }
#line 2045 "k0gram.tab.c"
    break;

  case 63: /* assignment_expr: IDENT MINUS_ASSIGN assignment_expr  */
#line 245 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_ASSIGNMENT_MINUS, "assignment_expr", 3, kids, NULL); }
#line 2051 "k0gram.tab.c"
    break;

  case 65: /* elvis_expr: logical_or_expr ELVIS elvis_expr  */
#line 249 "k0gram.y"
                                       {struct tree *kids[10] = {(yyvsp[-2].treeptr), (yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_ELVIS, "elvis_expr", 3, kids, NULL);}
#line 2057 "k0gram.tab.c"
    break;

  case 67: /* logical_or_expr: logical_or_expr OR logical_and_expr  */
#line 253 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_LOGICAL_OR_RECUR, "logical_or_expr", 3, kids, NULL); }
#line 2063 "k0gram.tab.c"
    break;

  case 69: /* logical_and_expr: logical_and_expr AND equality_expr  */
#line 257 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_LOGICAL_AND_RECUR, "logical_and_expr", 3, kids, NULL); }
#line 2069 "k0gram.tab.c"
    break;

  case 71: /* equality_expr: equality_expr EQ relational_expr  */
#line 261 "k0gram.y"
                                       {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_EQUALITY_EQ, "equality_expr", 3, kids, NULL); }
#line 2075 "k0gram.tab.c"
    break;

  case 72: /* equality_expr: equality_expr NEQ relational_expr  */
#line 262 "k0gram.y"
                                        {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_EQUALITY_NEQ, "equality_expr", 3, kids, NULL); }
#line 2081 "k0gram.tab.c"
    break;

  case 73: /* equality_expr: equality_expr REQ relational_expr  */
#line 263 "k0gram.y"
                                        {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_EQUALITY_REQ, "equality_expr", 3, kids, NULL); }
#line 2087 "k0gram.tab.c"
    break;

  case 74: /* equality_expr: equality_expr RNEQ relational_expr  */
#line 264 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_EQUALITY_RNEQ, "equality_expr", 3, kids, NULL); }
#line 2093 "k0gram.tab.c"
    break;

  case 76: /* relational_expr: relational_expr LT range_expr  */
#line 268 "k0gram.y"
                                    {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_RELATIONAL_LT, "relational_expr", 3, kids, NULL); }
#line 2099 "k0gram.tab.c"
    break;

  case 77: /* relational_expr: relational_expr GT range_expr  */
#line 269 "k0gram.y"
                                    {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_RELATIONAL_GT, "relational_expr", 3, kids, NULL); }
#line 2105 "k0gram.tab.c"
    break;

  case 78: /* relational_expr: relational_expr LTE range_expr  */
#line 270 "k0gram.y"
                                     {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_RELATIONAL_LTE, "relational_expr", 3, kids, NULL); }
#line 2111 "k0gram.tab.c"
    break;

  case 79: /* relational_expr: relational_expr GTE range_expr  */
#line 271 "k0gram.y"
                                     {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_RELATIONAL_GTE, "relational_expr", 3, kids, NULL); }
#line 2117 "k0gram.tab.c"
    break;

  case 81: /* range_expr: range_expr RANGE_INCL additive_expr  */
#line 275 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_RANGE_INCL, "range_expr", 3, kids, NULL);}
#line 2123 "k0gram.tab.c"
    break;

  case 82: /* range_expr: range_expr RANGE_EXCL additive_expr  */
#line 276 "k0gram.y"
                                          {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_RANGE_EXCL, "range_expr", 3, kids, NULL);}
#line 2129 "k0gram.tab.c"
    break;

  case 84: /* additive_expr: additive_expr PLUS multiplicative_expr  */
#line 280 "k0gram.y"
                                             {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_ADDITIVE_PLUS, "additive_expr", 3, kids, NULL); }
#line 2135 "k0gram.tab.c"
    break;

  case 85: /* additive_expr: additive_expr MINUS multiplicative_expr  */
#line 281 "k0gram.y"
                                              {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr), (yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_ADDITIVE_MINUS, "additive_expr", 3, kids, NULL); }
#line 2141 "k0gram.tab.c"
    break;

  case 87: /* multiplicative_expr: multiplicative_expr MUL unary_expr  */
#line 285 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_MULT_MUL, "multiplicative_expr", 3, kids, NULL); }
#line 2147 "k0gram.tab.c"
    break;

  case 88: /* multiplicative_expr: multiplicative_expr DIV unary_expr  */
#line 286 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_MULT_DIV, "multiplicative_expr", 3, kids, NULL); }
#line 2153 "k0gram.tab.c"
    break;

  case 89: /* multiplicative_expr: multiplicative_expr MOD unary_expr  */
#line 287 "k0gram.y"
                                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_MULT_MOD, "multiplicative_expr", 3, kids, NULL); }
#line 2159 "k0gram.tab.c"
    break;

  case 91: /* unary_expr: MINUS unary_expr  */
#line 291 "k0gram.y"
                       {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_UNARY_MINUS, "unary_expr", 2, kids, NULL); }
#line 2165 "k0gram.tab.c"
    break;

  case 92: /* unary_expr: NOT unary_expr  */
#line 292 "k0gram.y"
                     {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_UNARY_NOT, "unary_expr", 2, kids, NULL); }
#line 2171 "k0gram.tab.c"
    break;

  case 93: /* unary_expr: INC unary_expr  */
#line 293 "k0gram.y"
                     {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_UNARY_INC, "unary_expr", 2, kids, NULL); }
#line 2177 "k0gram.tab.c"
    break;

  case 94: /* unary_expr: DEC unary_expr  */
#line 294 "k0gram.y"
                     {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_UNARY_DEC, "unary_expr", 2, kids, NULL); }
#line 2183 "k0gram.tab.c"
    break;

  case 95: /* unary_expr: TYPE_CAST unary_expr  */
#line 295 "k0gram.y"
                           {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_UNARY_CAST, "unary_expr", 2, kids, NULL);}
#line 2189 "k0gram.tab.c"
    break;

  case 97: /* postfix_expr: postfix_expr INC  */
#line 299 "k0gram.y"
                       {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_POST_FIX_INC, "postfix_expr", 2, kids, NULL);}
#line 2195 "k0gram.tab.c"
    break;

  case 98: /* postfix_expr: postfix_expr DEC  */
#line 300 "k0gram.y"
                       {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_POST_FIX_DEC, "postfix_expr", 2, kids, NULL);}
#line 2201 "k0gram.tab.c"
    break;

  case 99: /* postfix_expr: postfix_expr NN_ASSERT  */
#line 301 "k0gram.y"
                             {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_POST_FIX_NN_ASSERT, "postfix_expr", 2, kids, NULL);}
#line 2207 "k0gram.tab.c"
    break;

  case 100: /* postfix_expr: postfix_expr NULLABLE  */
#line 302 "k0gram.y"
                            {struct tree *kids[10] = {(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_POST_FIX_NULLABLE, "postfix_expr", 2, kids, NULL);}
#line 2213 "k0gram.tab.c"
    break;

  case 101: /* postfix_expr: postfix_expr DOT IDENT  */
#line 303 "k0gram.y"
                             {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_POST_FIX_DOT, "postfix_expr", 3, kids, NULL);}
#line 2219 "k0gram.tab.c"
    break;

  case 102: /* postfix_expr: postfix_expr SAFE_CALL IDENT  */
#line 304 "k0gram.y"
                                   {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_POST_FIX_SAFE_CALL, "postfix_expr", 3, kids, NULL);}
#line 2225 "k0gram.tab.c"
    break;

  case 106: /* primary_expr: LPAREN expr RPAREN  */
#line 310 "k0gram.y"
                         {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_PRIMARY_PAREN, "primary_expr", 3, kids, NULL); }
#line 2231 "k0gram.tab.c"
    break;

  case 108: /* function_call: IDENT LPAREN function_call_values_list RPAREN  */
#line 314 "k0gram.y"
                                                    {struct tree *kids[10] = {(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_CALL, "function_call", 4, kids, NULL); }
#line 2237 "k0gram.tab.c"
    break;

  case 110: /* function_call_values_list: function_call_values_list COMMA functionCallVal  */
#line 318 "k0gram.y"
                                                      {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_CALL_VALUES_RECUR, "function_call_values_list", 3, kids, NULL); }
#line 2243 "k0gram.tab.c"
    break;

  case 111: /* function_call_values_list: %empty  */
#line 319 "k0gram.y"
      {(yyval.treeptr) = NULL; }
#line 2249 "k0gram.tab.c"
    break;

  case 114: /* functionCallVal: LPAREN literal RPAREN DOT function_call  */
#line 324 "k0gram.y"
                                              {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FUNCTION_CALL_OBJECT_CALL, "functionCallVal", 5, kids, NULL);}
#line 2255 "k0gram.tab.c"
    break;

  case 115: /* block: LBRACE statement_list RBRACE  */
#line 327 "k0gram.y"
                                   {struct tree *kids[10] = {(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_BLOCK, "block", 3, kids, NULL); }
#line 2261 "k0gram.tab.c"
    break;

  case 118: /* for_statement: FOR LPAREN IDENT IN IDENT RANGE_INCL IDENT RPAREN control_structure_body  */
#line 334 "k0gram.y"
                                                                               {struct tree *kids[10] = {(yyvsp[-8].treeptr),(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FOR_IDENT_IDENT, "for_statement", 9, kids, NULL); }
#line 2267 "k0gram.tab.c"
    break;

  case 119: /* for_statement: FOR LPAREN IDENT IN literal RANGE_INCL IDENT RPAREN control_structure_body  */
#line 335 "k0gram.y"
                                                                                 {struct tree *kids[10] = {(yyvsp[-8].treeptr),(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FOR_LITERAL_IDENT, "for_statement", 9, kids, NULL); }
#line 2273 "k0gram.tab.c"
    break;

  case 120: /* for_statement: FOR LPAREN IDENT IN IDENT RANGE_INCL literal RPAREN control_structure_body  */
#line 336 "k0gram.y"
                                                                                 {struct tree *kids[10] = {(yyvsp[-8].treeptr),(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FOR_IDENT_LITERAL, "for_statement", 9, kids, NULL); }
#line 2279 "k0gram.tab.c"
    break;

  case 121: /* for_statement: FOR LPAREN IDENT IN literal RANGE_INCL literal RPAREN control_structure_body  */
#line 337 "k0gram.y"
                                                                                   {struct tree *kids[10] = {(yyvsp[-8].treeptr),(yyvsp[-7].treeptr),(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_FOR_LITERAL_LITERAL, "for_statement", 9, kids, NULL); }
#line 2285 "k0gram.tab.c"
    break;

  case 122: /* while_statement: WHILE LPAREN expr RPAREN control_structure_body  */
#line 340 "k0gram.y"
                                                      {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_WHILE, "while_statement", 5, kids, NULL); }
#line 2291 "k0gram.tab.c"
    break;

  case 125: /* if_statement: IF LPAREN expr RPAREN control_structure_body  */
#line 347 "k0gram.y"
                                                             {struct tree *kids[10] = {(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_IF_SIMPLE, "if_statement", 5, kids, NULL); }
#line 2297 "k0gram.tab.c"
    break;

  case 126: /* if_statement: IF LPAREN expr RPAREN control_structure_body ELSE control_structure_body  */
#line 348 "k0gram.y"
                                                                               {struct tree *kids[10] = {(yyvsp[-6].treeptr),(yyvsp[-5].treeptr),(yyvsp[-4].treeptr),(yyvsp[-3].treeptr),(yyvsp[-2].treeptr),(yyvsp[-1].treeptr),(yyvsp[0].treeptr)}; (yyval.treeptr) = alctree(PR_IF_ELSE, "if_statement", 7, kids, NULL); }
#line 2303 "k0gram.tab.c"
    break;


#line 2307 "k0gram.tab.c"

        default: break;
      }
    if (yychar_backup != yychar)
      YY_LAC_DISCARD ("yychar change");
  }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yyesa, &yyes, &yyes_capacity, yytoken};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        if (yychar != YYEMPTY)
          YY_LAC_ESTABLISH;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  /* If the stack popping above didn't lose the initial context for the
     current lookahead token, the shift below will for sure.  */
  YY_LAC_DISCARD ("error recovery");

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yyes != yyesa)
    YYSTACK_FREE (yyes);
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 350 "k0gram.y"


struct tree *alctree(int prodrule, char *symbolname, int nkids, struct tree *kids[10], struct token *leaf) {
    struct tree *t = malloc(sizeof(struct tree));
    if (t == NULL) {
        fprintf(stderr, "Failed to allocate memory for tree node\n");
        exit(1);
    }
    t->id = serial++;
    t->prodrule = prodrule;
    t->symbolname = strdup(symbolname);
    t->nkids = nkids;
    t->leaf = leaf;
    t->type = NULL;

    // fill with NULL kids first before inserting real kids so that none of them are garbage
    for (int i = 0; i < 10; i++) {
        t->kids[i] = NULL;
    }
    // fill with real kids
    for (int i = 0; i < nkids; i++) {
        t->kids[i] = kids[i];
    }
    return t;
}
void print_node(struct tree *t) {
    printf("Prodrule: %d\nSymbol Name: %s\nNumber Kids: %d\n", t->prodrule, t->symbolname, t->nkids);
    if (t->leaf != NULL) {
        printf("Category: %d\nText: %s\nLine Number: %d\nFilename: %s\nival: %d\ndval: %f\nsval: %s\n\n",
        t->leaf->category, t->leaf->text, t->leaf->lineno, t->leaf->filename, t->leaf->ival, t->leaf->dval, t->leaf->sval);
    }
}

