/* A Bison parser, made by GNU Bison 3.7.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.7.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1


/* Substitute the variable and function names.  */
#define yyparse         latteparse
#define yylex           lattelex
#define yyerror         latteerror
#define yydebug         lattedebug
#define yynerrs         lattenerrs
#define yylval          lattelval
#define yychar          lattechar
#define yylloc          lattelloc

/* First part of user prologue.  */
#line 3 "latte.y"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Absyn.h"

#define YYMAXDEPTH 10000000

typedef struct latte_buffer_state *YY_BUFFER_STATE;
YY_BUFFER_STATE latte_scan_string(const char *str);
void latte_delete_buffer(YY_BUFFER_STATE buf);
extern int yyparse(void);
extern int yylex(void);
extern int latte_init_lexer(FILE * inp);
extern void yyerror(const char *str);

ListTopDef reverseListTopDef(ListTopDef l)
{
  ListTopDef prev = 0;
  ListTopDef tmp = 0;
  while (l)
  {
    tmp = l->listtopdef_;
    l->listtopdef_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListArg reverseListArg(ListArg l)
{
  ListArg prev = 0;
  ListArg tmp = 0;
  while (l)
  {
    tmp = l->listarg_;
    l->listarg_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListStmt reverseListStmt(ListStmt l)
{
  ListStmt prev = 0;
  ListStmt tmp = 0;
  while (l)
  {
    tmp = l->liststmt_;
    l->liststmt_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListItem reverseListItem(ListItem l)
{
  ListItem prev = 0;
  ListItem tmp = 0;
  while (l)
  {
    tmp = l->listitem_;
    l->listitem_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListType reverseListType(ListType l)
{
  ListType prev = 0;
  ListType tmp = 0;
  while (l)
  {
    tmp = l->listtype_;
    l->listtype_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListExpr reverseListExpr(ListExpr l)
{
  ListExpr prev = 0;
  ListExpr tmp = 0;
  while (l)
  {
    tmp = l->listexpr_;
    l->listexpr_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}

Program YY_RESULT_Program_ = 0;

Program pProgram(FILE *inp)
{
  latte_init_lexer(inp);
  int result = yyparse();
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}
Program psProgram(const char *str)
{
  YY_BUFFER_STATE buf;
  latte_init_lexer(0);
  buf = latte_scan_string(str);
  int result = yyparse();
  latte_delete_buffer(buf);
  if (result)
  { /* Failure */
    return 0;
  }
  else
  { /* Success */
    return YY_RESULT_Program_;
  }
}



#line 209 "Parser.c"

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


/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int lattedebug;
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
    _ERROR_ = 258,                 /* _ERROR_  */
    _SYMB_10 = 259,                /* _SYMB_10  */
    _SYMB_22 = 260,                /* _SYMB_22  */
    _SYMB_16 = 261,                /* _SYMB_16  */
    _SYMB_11 = 262,                /* _SYMB_11  */
    _SYMB_0 = 263,                 /* _SYMB_0  */
    _SYMB_1 = 264,                 /* _SYMB_1  */
    _SYMB_14 = 265,                /* _SYMB_14  */
    _SYMB_13 = 266,                /* _SYMB_13  */
    _SYMB_7 = 267,                 /* _SYMB_7  */
    _SYMB_2 = 268,                 /* _SYMB_2  */
    _SYMB_9 = 269,                 /* _SYMB_9  */
    _SYMB_8 = 270,                 /* _SYMB_8  */
    _SYMB_15 = 271,                /* _SYMB_15  */
    _SYMB_5 = 272,                 /* _SYMB_5  */
    _SYMB_17 = 273,                /* _SYMB_17  */
    _SYMB_18 = 274,                /* _SYMB_18  */
    _SYMB_6 = 275,                 /* _SYMB_6  */
    _SYMB_21 = 276,                /* _SYMB_21  */
    _SYMB_19 = 277,                /* _SYMB_19  */
    _SYMB_20 = 278,                /* _SYMB_20  */
    _SYMB_23 = 279,                /* _SYMB_23  */
    _SYMB_24 = 280,                /* _SYMB_24  */
    _SYMB_25 = 281,                /* _SYMB_25  */
    _SYMB_26 = 282,                /* _SYMB_26  */
    _SYMB_27 = 283,                /* _SYMB_27  */
    _SYMB_28 = 284,                /* _SYMB_28  */
    _SYMB_29 = 285,                /* _SYMB_29  */
    _SYMB_30 = 286,                /* _SYMB_30  */
    _SYMB_31 = 287,                /* _SYMB_31  */
    _SYMB_32 = 288,                /* _SYMB_32  */
    _SYMB_3 = 289,                 /* _SYMB_3  */
    _SYMB_12 = 290,                /* _SYMB_12  */
    _SYMB_4 = 291,                 /* _SYMB_4  */
    _STRING_ = 292,                /* _STRING_  */
    _INTEGER_ = 293,               /* _INTEGER_  */
    _IDENT_ = 294                  /* _IDENT_  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 134 "latte.y"

  int    _int;
  char   _char;
  double _double;
  char*  _string;
  Program program_;
  TopDef topdef_;
  ListTopDef listtopdef_;
  Arg arg_;
  ListArg listarg_;
  Block block_;
  ListStmt liststmt_;
  Stmt stmt_;
  Item item_;
  ListItem listitem_;
  Type type_;
  ListType listtype_;
  Expr expr_;
  ListExpr listexpr_;
  AddOp addop_;
  MulOp mulop_;
  RelOp relop_;

#line 319 "Parser.c"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE lattelval;
extern YYLTYPE lattelloc;
int latteparse (void);


/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL__ERROR_ = 3,                    /* _ERROR_  */
  YYSYMBOL__SYMB_10 = 4,                   /* _SYMB_10  */
  YYSYMBOL__SYMB_22 = 5,                   /* _SYMB_22  */
  YYSYMBOL__SYMB_16 = 6,                   /* _SYMB_16  */
  YYSYMBOL__SYMB_11 = 7,                   /* _SYMB_11  */
  YYSYMBOL__SYMB_0 = 8,                    /* _SYMB_0  */
  YYSYMBOL__SYMB_1 = 9,                    /* _SYMB_1  */
  YYSYMBOL__SYMB_14 = 10,                  /* _SYMB_14  */
  YYSYMBOL__SYMB_13 = 11,                  /* _SYMB_13  */
  YYSYMBOL__SYMB_7 = 12,                   /* _SYMB_7  */
  YYSYMBOL__SYMB_2 = 13,                   /* _SYMB_2  */
  YYSYMBOL__SYMB_9 = 14,                   /* _SYMB_9  */
  YYSYMBOL__SYMB_8 = 15,                   /* _SYMB_8  */
  YYSYMBOL__SYMB_15 = 16,                  /* _SYMB_15  */
  YYSYMBOL__SYMB_5 = 17,                   /* _SYMB_5  */
  YYSYMBOL__SYMB_17 = 18,                  /* _SYMB_17  */
  YYSYMBOL__SYMB_18 = 19,                  /* _SYMB_18  */
  YYSYMBOL__SYMB_6 = 20,                   /* _SYMB_6  */
  YYSYMBOL__SYMB_21 = 21,                  /* _SYMB_21  */
  YYSYMBOL__SYMB_19 = 22,                  /* _SYMB_19  */
  YYSYMBOL__SYMB_20 = 23,                  /* _SYMB_20  */
  YYSYMBOL__SYMB_23 = 24,                  /* _SYMB_23  */
  YYSYMBOL__SYMB_24 = 25,                  /* _SYMB_24  */
  YYSYMBOL__SYMB_25 = 26,                  /* _SYMB_25  */
  YYSYMBOL__SYMB_26 = 27,                  /* _SYMB_26  */
  YYSYMBOL__SYMB_27 = 28,                  /* _SYMB_27  */
  YYSYMBOL__SYMB_28 = 29,                  /* _SYMB_28  */
  YYSYMBOL__SYMB_29 = 30,                  /* _SYMB_29  */
  YYSYMBOL__SYMB_30 = 31,                  /* _SYMB_30  */
  YYSYMBOL__SYMB_31 = 32,                  /* _SYMB_31  */
  YYSYMBOL__SYMB_32 = 33,                  /* _SYMB_32  */
  YYSYMBOL__SYMB_3 = 34,                   /* _SYMB_3  */
  YYSYMBOL__SYMB_12 = 35,                  /* _SYMB_12  */
  YYSYMBOL__SYMB_4 = 36,                   /* _SYMB_4  */
  YYSYMBOL__STRING_ = 37,                  /* _STRING_  */
  YYSYMBOL__INTEGER_ = 38,                 /* _INTEGER_  */
  YYSYMBOL__IDENT_ = 39,                   /* _IDENT_  */
  YYSYMBOL_YYACCEPT = 40,                  /* $accept  */
  YYSYMBOL_Program = 41,                   /* Program  */
  YYSYMBOL_TopDef = 42,                    /* TopDef  */
  YYSYMBOL_ListTopDef = 43,                /* ListTopDef  */
  YYSYMBOL_Arg = 44,                       /* Arg  */
  YYSYMBOL_ListArg = 45,                   /* ListArg  */
  YYSYMBOL_Block = 46,                     /* Block  */
  YYSYMBOL_ListStmt = 47,                  /* ListStmt  */
  YYSYMBOL_Stmt = 48,                      /* Stmt  */
  YYSYMBOL_Item = 49,                      /* Item  */
  YYSYMBOL_ListItem = 50,                  /* ListItem  */
  YYSYMBOL_Type = 51,                      /* Type  */
  YYSYMBOL_Expr6 = 52,                     /* Expr6  */
  YYSYMBOL_Expr5 = 53,                     /* Expr5  */
  YYSYMBOL_Expr4 = 54,                     /* Expr4  */
  YYSYMBOL_Expr3 = 55,                     /* Expr3  */
  YYSYMBOL_Expr2 = 56,                     /* Expr2  */
  YYSYMBOL_Expr1 = 57,                     /* Expr1  */
  YYSYMBOL_Expr = 58,                      /* Expr  */
  YYSYMBOL_ListExpr = 59,                  /* ListExpr  */
  YYSYMBOL_AddOp = 60,                     /* AddOp  */
  YYSYMBOL_MulOp = 61,                     /* MulOp  */
  YYSYMBOL_RelOp = 62                      /* RelOp  */
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
typedef yytype_int8 yy_state_t;

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
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && ! defined __ICC && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                            \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
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

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

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
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   135

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  40
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  23
/* YYNRULES -- Number of rules.  */
#define YYNRULES  66
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  107

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   294


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
      35,    36,    37,    38,    39
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   223,   223,   225,   227,   228,   230,   232,   233,   234,
     236,   238,   239,   241,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   252,   254,   255,   257,   258,   260,
     261,   262,   263,   269,   270,   271,   272,   273,   274,   275,
     277,   278,   279,   281,   282,   284,   285,   287,   288,   290,
     291,   293,   294,   296,   297,   298,   300,   301,   303,   304,
     305,   307,   308,   309,   310,   311,   312
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "_ERROR_", "_SYMB_10",
  "_SYMB_22", "_SYMB_16", "_SYMB_11", "_SYMB_0", "_SYMB_1", "_SYMB_14",
  "_SYMB_13", "_SYMB_7", "_SYMB_2", "_SYMB_9", "_SYMB_8", "_SYMB_15",
  "_SYMB_5", "_SYMB_17", "_SYMB_18", "_SYMB_6", "_SYMB_21", "_SYMB_19",
  "_SYMB_20", "_SYMB_23", "_SYMB_24", "_SYMB_25", "_SYMB_26", "_SYMB_27",
  "_SYMB_28", "_SYMB_29", "_SYMB_30", "_SYMB_31", "_SYMB_32", "_SYMB_3",
  "_SYMB_12", "_SYMB_4", "_STRING_", "_INTEGER_", "_IDENT_", "$accept",
  "Program", "TopDef", "ListTopDef", "Arg", "ListArg", "Block", "ListStmt",
  "Stmt", "Item", "ListItem", "Type", "Expr6", "Expr5", "Expr4", "Expr3",
  "Expr2", "Expr1", "Expr", "ListExpr", "AddOp", "MulOp", "RelOp", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_int16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294
};
#endif

#define YYPACT_NINF (-86)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      73,   -86,   -86,   -86,   -86,     2,    73,   -86,   -30,   -86,
     -86,     5,    73,    10,    12,   -15,    73,    -6,   -86,   -86,
     -86,   -86,     8,    86,    96,    86,   -86,   -86,    19,    82,
     -86,    23,   -86,   -86,   -86,    45,   -86,   -86,     9,   -86,
     -86,    56,    -7,    -4,    15,    34,    48,   -86,    52,   -86,
      96,   -86,    51,    96,    96,    53,    54,    96,    49,    62,
      68,   -86,   -86,   -86,    96,   -86,   -86,    96,   -86,    96,
     -86,   -86,   -86,   -86,   -86,    96,    96,   -86,   -86,    83,
     -86,    84,    78,    89,   -86,   -86,    85,    96,     9,   -86,
     -86,    56,   -86,    -7,   -86,    50,    50,    96,   -86,   -86,
     -86,   -86,    70,   -86,   -86,    50,   -86
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    31,    29,    30,    32,     0,     4,     2,     0,     1,
       5,     0,     7,     8,     0,     0,     7,     0,     6,     9,
      11,     3,     0,     0,     0,     0,    13,    36,     0,     0,
      35,     0,    10,    38,    34,    33,    14,    12,     0,    42,
      44,    46,    48,    50,    52,     0,    33,    41,     0,    40,
       0,    20,     0,     0,    53,     0,     0,     0,    25,    27,
       0,    60,    58,    59,     0,    56,    57,     0,    66,     0,
      61,    62,    65,    63,    64,     0,     0,    24,    39,     0,
      19,     0,    54,     0,    17,    18,     0,     0,     0,    15,
      43,    45,    49,    47,    51,     0,     0,    53,    37,    16,
      26,    28,    21,    23,    55,     0,    22
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -86,   -86,   -86,   100,   -86,    91,    92,   -86,   -85,   -86,
      26,    43,   -17,    47,    59,    40,   -86,    60,   -24,    21,
     -86,   -86,   -86
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     5,     6,     7,    13,    14,    36,    22,    37,    59,
      60,    38,    39,    40,    41,    42,    43,    44,    45,    83,
      67,    64,    75
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int8 yytable[] =
{
      48,    68,     9,    69,    65,    52,    47,    66,    49,    11,
     102,   103,    23,    12,    70,    71,    24,    72,    73,    74,
     106,    17,    25,    16,    18,    26,    79,    50,    20,    81,
      82,    53,     1,    86,    27,    28,     2,    29,     3,    30,
       4,    31,    20,     8,    32,    33,    34,    35,    58,     8,
      76,    77,    94,    54,    23,    15,    54,    55,    24,    15,
      56,    78,    61,   100,    25,    57,    62,    26,    80,    87,
      84,    85,    63,    82,     1,    88,    27,    28,     2,    29,
       3,    30,     4,    31,    20,    89,    23,    33,    34,    35,
      24,    97,    95,    96,    24,   105,    25,     1,    98,    51,
      23,     2,    99,     3,    24,     4,    10,    19,    27,    21,
      25,    90,    27,    30,   101,    93,     0,    30,   104,    33,
      34,    46,    27,    33,    34,    46,    91,    30,     0,    92,
       0,     0,     0,    33,    34,    46
};

static const yytype_int8 yycheck[] =
{
      24,     5,     0,     7,    11,    29,    23,    14,    25,    39,
      95,    96,     4,     8,    18,    19,     8,    21,    22,    23,
     105,     9,    14,    13,    39,    17,    50,     8,    34,    53,
      54,     8,    24,    57,    26,    27,    28,    29,    30,    31,
      32,    33,    34,     0,    36,    37,    38,    39,    39,     6,
      35,    17,    76,     8,     4,    12,     8,    12,     8,    16,
      15,     9,     6,    87,    14,    20,    10,    17,    17,    20,
      17,    17,    16,    97,    24,    13,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    17,     4,    37,    38,    39,
       8,    13,     9,     9,     8,    25,    14,    24,     9,    17,
       4,    28,    17,    30,     8,    32,     6,    16,    26,    17,
      14,    64,    26,    31,    88,    75,    -1,    31,    97,    37,
      38,    39,    26,    37,    38,    39,    67,    31,    -1,    69,
      -1,    -1,    -1,    37,    38,    39
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    24,    28,    30,    32,    41,    42,    43,    51,     0,
      43,    39,     8,    44,    45,    51,    13,     9,    39,    45,
      34,    46,    47,     4,     8,    14,    17,    26,    27,    29,
      31,    33,    36,    37,    38,    39,    46,    48,    51,    52,
      53,    54,    55,    56,    57,    58,    39,    52,    58,    52,
       8,    17,    58,     8,     8,    12,    15,    20,    39,    49,
      50,     6,    10,    16,    61,    11,    14,    60,     5,     7,
      18,    19,    21,    22,    23,    62,    35,    17,     9,    58,
      17,    58,    58,    59,    17,    17,    58,    20,    13,    17,
      53,    54,    57,    55,    58,     9,     9,    13,     9,    17,
      58,    50,    48,    48,    59,    25,    48
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    40,    41,    42,    43,    43,    44,    45,    45,    45,
      46,    47,    47,    48,    48,    48,    48,    48,    48,    48,
      48,    48,    48,    48,    48,    49,    49,    50,    50,    51,
      51,    51,    51,    52,    52,    52,    52,    52,    52,    52,
      53,    53,    53,    54,    54,    55,    55,    56,    56,    57,
      57,    58,    58,    59,    59,    59,    60,    60,    61,    61,
      61,    62,    62,    62,    62,    62,    62
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     6,     1,     2,     2,     0,     1,     3,
       3,     0,     2,     1,     1,     3,     4,     3,     3,     3,
       2,     5,     7,     5,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     4,     1,     3,
       2,     2,     1,     3,     1,     3,     1,     3,     1,     3,
       1,     3,     1,     0,     1,     3,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YY_LOCATION_PRINT
#  if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
 }

#   define YY_LOCATION_PRINT(File, Loc)          \
  yy_location_print_ (File, &(Loc))

#  else
#   define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#  endif
# endif /* !defined YY_LOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YYUSE (yyoutput);
  YYUSE (yylocationp);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yykind < YYNTOKENS)
    YYPRINT (yyo, yytoknum[yykind], *yyvaluep);
# endif
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YY_LOCATION_PRINT (yyo, *yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YYUSE (yyvaluep);
  YYUSE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */
  yylsp[0] = yylloc;
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
    goto yyexhaustedlab;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
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
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ListTopDef  */
#line 223 "latte.y"
                     { (yyval.program_) = make_Prog((yyvsp[0].listtopdef_)); (yyval.program_)->line_number = (yyloc).first_line; (yyval.program_)->char_number = (yyloc).first_column; YY_RESULT_Program_= (yyval.program_); }
#line 1544 "Parser.c"
    break;

  case 3: /* TopDef: Type _IDENT_ _SYMB_0 ListArg _SYMB_1 Block  */
#line 225 "latte.y"
                                                    { (yyval.topdef_) = make_FnDef((yyvsp[-5].type_), (yyvsp[-4]._string), (yyvsp[-2].listarg_), (yyvsp[0].block_)); (yyval.topdef_)->line_number = (yyloc).first_line; (yyval.topdef_)->char_number = (yyloc).first_column;  }
#line 1550 "Parser.c"
    break;

  case 4: /* ListTopDef: TopDef  */
#line 227 "latte.y"
                    { (yyval.listtopdef_) = make_ListTopDef((yyvsp[0].topdef_), 0);  }
#line 1556 "Parser.c"
    break;

  case 5: /* ListTopDef: TopDef ListTopDef  */
#line 228 "latte.y"
                      { (yyval.listtopdef_) = make_ListTopDef((yyvsp[-1].topdef_), (yyvsp[0].listtopdef_));  }
#line 1562 "Parser.c"
    break;

  case 6: /* Arg: Type _IDENT_  */
#line 230 "latte.y"
                   { (yyval.arg_) = make_Ar((yyvsp[-1].type_), (yyvsp[0]._string)); (yyval.arg_)->line_number = (yyloc).first_line; (yyval.arg_)->char_number = (yyloc).first_column;  }
#line 1568 "Parser.c"
    break;

  case 7: /* ListArg: %empty  */
#line 232 "latte.y"
                      { (yyval.listarg_) = 0;  }
#line 1574 "Parser.c"
    break;

  case 8: /* ListArg: Arg  */
#line 233 "latte.y"
        { (yyval.listarg_) = make_ListArg((yyvsp[0].arg_), 0);  }
#line 1580 "Parser.c"
    break;

  case 9: /* ListArg: Arg _SYMB_2 ListArg  */
#line 234 "latte.y"
                        { (yyval.listarg_) = make_ListArg((yyvsp[-2].arg_), (yyvsp[0].listarg_));  }
#line 1586 "Parser.c"
    break;

  case 10: /* Block: _SYMB_3 ListStmt _SYMB_4  */
#line 236 "latte.y"
                                 { (yyval.block_) = make_Blk(reverseListStmt((yyvsp[-1].liststmt_))); (yyval.block_)->line_number = (yyloc).first_line; (yyval.block_)->char_number = (yyloc).first_column;  }
#line 1592 "Parser.c"
    break;

  case 11: /* ListStmt: %empty  */
#line 238 "latte.y"
                       { (yyval.liststmt_) = 0;  }
#line 1598 "Parser.c"
    break;

  case 12: /* ListStmt: ListStmt Stmt  */
#line 239 "latte.y"
                  { (yyval.liststmt_) = make_ListStmt((yyvsp[0].stmt_), (yyvsp[-1].liststmt_));  }
#line 1604 "Parser.c"
    break;

  case 13: /* Stmt: _SYMB_5  */
#line 241 "latte.y"
               { (yyval.stmt_) = make_Empty(); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1610 "Parser.c"
    break;

  case 14: /* Stmt: Block  */
#line 242 "latte.y"
          { (yyval.stmt_) = make_BStmt((yyvsp[0].block_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1616 "Parser.c"
    break;

  case 15: /* Stmt: Type ListItem _SYMB_5  */
#line 243 "latte.y"
                          { (yyval.stmt_) = make_Decl((yyvsp[-2].type_), (yyvsp[-1].listitem_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1622 "Parser.c"
    break;

  case 16: /* Stmt: _IDENT_ _SYMB_6 Expr _SYMB_5  */
#line 244 "latte.y"
                                 { (yyval.stmt_) = make_Ass((yyvsp[-3]._string), (yyvsp[-1].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1628 "Parser.c"
    break;

  case 17: /* Stmt: _IDENT_ _SYMB_7 _SYMB_5  */
#line 245 "latte.y"
                            { (yyval.stmt_) = make_Incr((yyvsp[-2]._string)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1634 "Parser.c"
    break;

  case 18: /* Stmt: _IDENT_ _SYMB_8 _SYMB_5  */
#line 246 "latte.y"
                            { (yyval.stmt_) = make_Decr((yyvsp[-2]._string)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1640 "Parser.c"
    break;

  case 19: /* Stmt: _SYMB_28 Expr _SYMB_5  */
#line 247 "latte.y"
                          { (yyval.stmt_) = make_Ret((yyvsp[-1].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1646 "Parser.c"
    break;

  case 20: /* Stmt: _SYMB_28 _SYMB_5  */
#line 248 "latte.y"
                     { (yyval.stmt_) = make_VRet(); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1652 "Parser.c"
    break;

  case 21: /* Stmt: _SYMB_26 _SYMB_0 Expr _SYMB_1 Stmt  */
#line 249 "latte.y"
                                       { (yyval.stmt_) = make_Cond((yyvsp[-2].expr_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1658 "Parser.c"
    break;

  case 22: /* Stmt: _SYMB_26 _SYMB_0 Expr _SYMB_1 Stmt _SYMB_24 Stmt  */
#line 250 "latte.y"
                                                     { (yyval.stmt_) = make_CondElse((yyvsp[-4].expr_), (yyvsp[-2].stmt_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1664 "Parser.c"
    break;

  case 23: /* Stmt: _SYMB_32 _SYMB_0 Expr _SYMB_1 Stmt  */
#line 251 "latte.y"
                                       { (yyval.stmt_) = make_While((yyvsp[-2].expr_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1670 "Parser.c"
    break;

  case 24: /* Stmt: Expr _SYMB_5  */
#line 252 "latte.y"
                 { (yyval.stmt_) = make_SExp((yyvsp[-1].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1676 "Parser.c"
    break;

  case 25: /* Item: _IDENT_  */
#line 254 "latte.y"
               { (yyval.item_) = make_NoInit((yyvsp[0]._string)); (yyval.item_)->line_number = (yyloc).first_line; (yyval.item_)->char_number = (yyloc).first_column;  }
#line 1682 "Parser.c"
    break;

  case 26: /* Item: _IDENT_ _SYMB_6 Expr  */
#line 255 "latte.y"
                         { (yyval.item_) = make_Init((yyvsp[-2]._string), (yyvsp[0].expr_)); (yyval.item_)->line_number = (yyloc).first_line; (yyval.item_)->char_number = (yyloc).first_column;  }
#line 1688 "Parser.c"
    break;

  case 27: /* ListItem: Item  */
#line 257 "latte.y"
                { (yyval.listitem_) = make_ListItem((yyvsp[0].item_), 0);  }
#line 1694 "Parser.c"
    break;

  case 28: /* ListItem: Item _SYMB_2 ListItem  */
#line 258 "latte.y"
                          { (yyval.listitem_) = make_ListItem((yyvsp[-2].item_), (yyvsp[0].listitem_));  }
#line 1700 "Parser.c"
    break;

  case 29: /* Type: _SYMB_27  */
#line 260 "latte.y"
                { (yyval.type_) = make_Int(); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1706 "Parser.c"
    break;

  case 30: /* Type: _SYMB_29  */
#line 261 "latte.y"
             { (yyval.type_) = make_Str(); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1712 "Parser.c"
    break;

  case 31: /* Type: _SYMB_23  */
#line 262 "latte.y"
             { (yyval.type_) = make_Bool(); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1718 "Parser.c"
    break;

  case 32: /* Type: _SYMB_31  */
#line 263 "latte.y"
             { (yyval.type_) = make_Void(); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1724 "Parser.c"
    break;

  case 33: /* Expr6: _IDENT_  */
#line 269 "latte.y"
                { (yyval.expr_) = make_EVar((yyvsp[0]._string)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1730 "Parser.c"
    break;

  case 34: /* Expr6: _INTEGER_  */
#line 270 "latte.y"
              { (yyval.expr_) = make_ELitInt((yyvsp[0]._int)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1736 "Parser.c"
    break;

  case 35: /* Expr6: _SYMB_30  */
#line 271 "latte.y"
             { (yyval.expr_) = make_ELitTrue(); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1742 "Parser.c"
    break;

  case 36: /* Expr6: _SYMB_25  */
#line 272 "latte.y"
             { (yyval.expr_) = make_ELitFalse(); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1748 "Parser.c"
    break;

  case 37: /* Expr6: _IDENT_ _SYMB_0 ListExpr _SYMB_1  */
#line 273 "latte.y"
                                     { (yyval.expr_) = make_EApp((yyvsp[-3]._string), (yyvsp[-1].listexpr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1754 "Parser.c"
    break;

  case 38: /* Expr6: _STRING_  */
#line 274 "latte.y"
             { (yyval.expr_) = make_EString((yyvsp[0]._string)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1760 "Parser.c"
    break;

  case 39: /* Expr6: _SYMB_0 Expr _SYMB_1  */
#line 275 "latte.y"
                         { (yyval.expr_) = (yyvsp[-1].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1766 "Parser.c"
    break;

  case 40: /* Expr5: _SYMB_9 Expr6  */
#line 277 "latte.y"
                      { (yyval.expr_) = make_Neg((yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1772 "Parser.c"
    break;

  case 41: /* Expr5: _SYMB_10 Expr6  */
#line 278 "latte.y"
                   { (yyval.expr_) = make_Not((yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1778 "Parser.c"
    break;

  case 42: /* Expr5: Expr6  */
#line 279 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1784 "Parser.c"
    break;

  case 43: /* Expr4: Expr4 MulOp Expr5  */
#line 281 "latte.y"
                          { (yyval.expr_) = make_EMul((yyvsp[-2].expr_), (yyvsp[-1].mulop_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1790 "Parser.c"
    break;

  case 44: /* Expr4: Expr5  */
#line 282 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1796 "Parser.c"
    break;

  case 45: /* Expr3: Expr3 AddOp Expr4  */
#line 284 "latte.y"
                          { (yyval.expr_) = make_EAdd((yyvsp[-2].expr_), (yyvsp[-1].addop_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1802 "Parser.c"
    break;

  case 46: /* Expr3: Expr4  */
#line 285 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1808 "Parser.c"
    break;

  case 47: /* Expr2: Expr2 RelOp Expr3  */
#line 287 "latte.y"
                          { (yyval.expr_) = make_ERel((yyvsp[-2].expr_), (yyvsp[-1].relop_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1814 "Parser.c"
    break;

  case 48: /* Expr2: Expr3  */
#line 288 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1820 "Parser.c"
    break;

  case 49: /* Expr1: Expr2 _SYMB_11 Expr1  */
#line 290 "latte.y"
                             { (yyval.expr_) = make_EAnd((yyvsp[-2].expr_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1826 "Parser.c"
    break;

  case 50: /* Expr1: Expr2  */
#line 291 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1832 "Parser.c"
    break;

  case 51: /* Expr: Expr1 _SYMB_12 Expr  */
#line 293 "latte.y"
                           { (yyval.expr_) = make_EOr((yyvsp[-2].expr_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1838 "Parser.c"
    break;

  case 52: /* Expr: Expr1  */
#line 294 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1844 "Parser.c"
    break;

  case 53: /* ListExpr: %empty  */
#line 296 "latte.y"
                       { (yyval.listexpr_) = 0;  }
#line 1850 "Parser.c"
    break;

  case 54: /* ListExpr: Expr  */
#line 297 "latte.y"
         { (yyval.listexpr_) = make_ListExpr((yyvsp[0].expr_), 0);  }
#line 1856 "Parser.c"
    break;

  case 55: /* ListExpr: Expr _SYMB_2 ListExpr  */
#line 298 "latte.y"
                          { (yyval.listexpr_) = make_ListExpr((yyvsp[-2].expr_), (yyvsp[0].listexpr_));  }
#line 1862 "Parser.c"
    break;

  case 56: /* AddOp: _SYMB_13  */
#line 300 "latte.y"
                 { (yyval.addop_) = make_Plus(); (yyval.addop_)->line_number = (yyloc).first_line; (yyval.addop_)->char_number = (yyloc).first_column;  }
#line 1868 "Parser.c"
    break;

  case 57: /* AddOp: _SYMB_9  */
#line 301 "latte.y"
            { (yyval.addop_) = make_Minus(); (yyval.addop_)->line_number = (yyloc).first_line; (yyval.addop_)->char_number = (yyloc).first_column;  }
#line 1874 "Parser.c"
    break;

  case 58: /* MulOp: _SYMB_14  */
#line 303 "latte.y"
                 { (yyval.mulop_) = make_Times(); (yyval.mulop_)->line_number = (yyloc).first_line; (yyval.mulop_)->char_number = (yyloc).first_column;  }
#line 1880 "Parser.c"
    break;

  case 59: /* MulOp: _SYMB_15  */
#line 304 "latte.y"
             { (yyval.mulop_) = make_Div(); (yyval.mulop_)->line_number = (yyloc).first_line; (yyval.mulop_)->char_number = (yyloc).first_column;  }
#line 1886 "Parser.c"
    break;

  case 60: /* MulOp: _SYMB_16  */
#line 305 "latte.y"
             { (yyval.mulop_) = make_Mod(); (yyval.mulop_)->line_number = (yyloc).first_line; (yyval.mulop_)->char_number = (yyloc).first_column;  }
#line 1892 "Parser.c"
    break;

  case 61: /* RelOp: _SYMB_17  */
#line 307 "latte.y"
                 { (yyval.relop_) = make_LTH(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 1898 "Parser.c"
    break;

  case 62: /* RelOp: _SYMB_18  */
#line 308 "latte.y"
             { (yyval.relop_) = make_LE(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 1904 "Parser.c"
    break;

  case 63: /* RelOp: _SYMB_19  */
#line 309 "latte.y"
             { (yyval.relop_) = make_GTH(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 1910 "Parser.c"
    break;

  case 64: /* RelOp: _SYMB_20  */
#line 310 "latte.y"
             { (yyval.relop_) = make_GE(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 1916 "Parser.c"
    break;

  case 65: /* RelOp: _SYMB_21  */
#line 311 "latte.y"
             { (yyval.relop_) = make_EQU(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 1922 "Parser.c"
    break;

  case 66: /* RelOp: _SYMB_22  */
#line 312 "latte.y"
             { (yyval.relop_) = make_NE(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 1928 "Parser.c"
    break;


#line 1932 "Parser.c"

      default: break;
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
  *++yylsp = yyloc;

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
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;


#if !defined yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturn;
#endif


/*-------------------------------------------------------.
| yyreturn -- parsing is finished, clean up and return.  |
`-------------------------------------------------------*/
yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 315 "latte.y"

void __attribute__((weak)) yyerror(const char *str)
{
  extern char *lattetext;
  fprintf(stderr,"error: %d,%d: %s at %s\n",
  lattelloc.first_line, lattelloc.first_column, str, lattetext);
}

