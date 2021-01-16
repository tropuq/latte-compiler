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
ListClassDecl reverseListClassDecl(ListClassDecl l)
{
  ListClassDecl prev = 0;
  ListClassDecl tmp = 0;
  while (l)
  {
    tmp = l->listclassdecl_;
    l->listclassdecl_ = prev;
    prev = l;
    l = tmp;
  }
  return prev;
}
ListIdent reverseListIdent(ListIdent l)
{
  ListIdent prev = 0;
  ListIdent tmp = 0;
  while (l)
  {
    tmp = l->listident_;
    l->listident_ = prev;
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
ListField reverseListField(ListField l)
{
  ListField prev = 0;
  ListField tmp = 0;
  while (l)
  {
    tmp = l->listfield_;
    l->listfield_ = prev;
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



#line 248 "Parser.c"

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
    _SYMB_15 = 259,                /* _SYMB_15  */
    _SYMB_27 = 260,                /* _SYMB_27  */
    _SYMB_21 = 261,                /* _SYMB_21  */
    _SYMB_16 = 262,                /* _SYMB_16  */
    _SYMB_0 = 263,                 /* _SYMB_0  */
    _SYMB_1 = 264,                 /* _SYMB_1  */
    _SYMB_19 = 265,                /* _SYMB_19  */
    _SYMB_18 = 266,                /* _SYMB_18  */
    _SYMB_7 = 267,                 /* _SYMB_7  */
    _SYMB_2 = 268,                 /* _SYMB_2  */
    _SYMB_14 = 269,                /* _SYMB_14  */
    _SYMB_8 = 270,                 /* _SYMB_8  */
    _SYMB_13 = 271,                /* _SYMB_13  */
    _SYMB_20 = 272,                /* _SYMB_20  */
    _SYMB_9 = 273,                 /* _SYMB_9  */
    _SYMB_5 = 274,                 /* _SYMB_5  */
    _SYMB_22 = 275,                /* _SYMB_22  */
    _SYMB_23 = 276,                /* _SYMB_23  */
    _SYMB_6 = 277,                 /* _SYMB_6  */
    _SYMB_26 = 278,                /* _SYMB_26  */
    _SYMB_24 = 279,                /* _SYMB_24  */
    _SYMB_25 = 280,                /* _SYMB_25  */
    _SYMB_11 = 281,                /* _SYMB_11  */
    _SYMB_10 = 282,                /* _SYMB_10  */
    _SYMB_12 = 283,                /* _SYMB_12  */
    _SYMB_28 = 284,                /* _SYMB_28  */
    _SYMB_29 = 285,                /* _SYMB_29  */
    _SYMB_30 = 286,                /* _SYMB_30  */
    _SYMB_31 = 287,                /* _SYMB_31  */
    _SYMB_32 = 288,                /* _SYMB_32  */
    _SYMB_33 = 289,                /* _SYMB_33  */
    _SYMB_34 = 290,                /* _SYMB_34  */
    _SYMB_35 = 291,                /* _SYMB_35  */
    _SYMB_36 = 292,                /* _SYMB_36  */
    _SYMB_37 = 293,                /* _SYMB_37  */
    _SYMB_38 = 294,                /* _SYMB_38  */
    _SYMB_39 = 295,                /* _SYMB_39  */
    _SYMB_40 = 296,                /* _SYMB_40  */
    _SYMB_41 = 297,                /* _SYMB_41  */
    _SYMB_42 = 298,                /* _SYMB_42  */
    _SYMB_43 = 299,                /* _SYMB_43  */
    _SYMB_3 = 300,                 /* _SYMB_3  */
    _SYMB_17 = 301,                /* _SYMB_17  */
    _SYMB_4 = 302,                 /* _SYMB_4  */
    _STRING_ = 303,                /* _STRING_  */
    _INTEGER_ = 304,               /* _INTEGER_  */
    _IDENT_ = 305                  /* _IDENT_  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 173 "latte.y"

  int    _int;
  char   _char;
  double _double;
  char*  _string;
  Program program_;
  TopDef topdef_;
  ListTopDef listtopdef_;
  Arg arg_;
  ListArg listarg_;
  ClassBlock classblock_;
  ListClassDecl listclassdecl_;
  ClassDecl classdecl_;
  ListIdent listident_;
  Block block_;
  ListStmt liststmt_;
  Stmt stmt_;
  Item item_;
  ListItem listitem_;
  TypeSimple typesimple_;
  Type type_;
  ListType listtype_;
  FieldVal fieldval_;
  Field field_;
  ListField listfield_;
  Expr expr_;
  ListExpr listexpr_;
  AddOp addop_;
  MulOp mulop_;
  RelOp relop_;

#line 377 "Parser.c"

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
  YYSYMBOL__SYMB_15 = 4,                   /* _SYMB_15  */
  YYSYMBOL__SYMB_27 = 5,                   /* _SYMB_27  */
  YYSYMBOL__SYMB_21 = 6,                   /* _SYMB_21  */
  YYSYMBOL__SYMB_16 = 7,                   /* _SYMB_16  */
  YYSYMBOL__SYMB_0 = 8,                    /* _SYMB_0  */
  YYSYMBOL__SYMB_1 = 9,                    /* _SYMB_1  */
  YYSYMBOL__SYMB_19 = 10,                  /* _SYMB_19  */
  YYSYMBOL__SYMB_18 = 11,                  /* _SYMB_18  */
  YYSYMBOL__SYMB_7 = 12,                   /* _SYMB_7  */
  YYSYMBOL__SYMB_2 = 13,                   /* _SYMB_2  */
  YYSYMBOL__SYMB_14 = 14,                  /* _SYMB_14  */
  YYSYMBOL__SYMB_8 = 15,                   /* _SYMB_8  */
  YYSYMBOL__SYMB_13 = 16,                  /* _SYMB_13  */
  YYSYMBOL__SYMB_20 = 17,                  /* _SYMB_20  */
  YYSYMBOL__SYMB_9 = 18,                   /* _SYMB_9  */
  YYSYMBOL__SYMB_5 = 19,                   /* _SYMB_5  */
  YYSYMBOL__SYMB_22 = 20,                  /* _SYMB_22  */
  YYSYMBOL__SYMB_23 = 21,                  /* _SYMB_23  */
  YYSYMBOL__SYMB_6 = 22,                   /* _SYMB_6  */
  YYSYMBOL__SYMB_26 = 23,                  /* _SYMB_26  */
  YYSYMBOL__SYMB_24 = 24,                  /* _SYMB_24  */
  YYSYMBOL__SYMB_25 = 25,                  /* _SYMB_25  */
  YYSYMBOL__SYMB_11 = 26,                  /* _SYMB_11  */
  YYSYMBOL__SYMB_10 = 27,                  /* _SYMB_10  */
  YYSYMBOL__SYMB_12 = 28,                  /* _SYMB_12  */
  YYSYMBOL__SYMB_28 = 29,                  /* _SYMB_28  */
  YYSYMBOL__SYMB_29 = 30,                  /* _SYMB_29  */
  YYSYMBOL__SYMB_30 = 31,                  /* _SYMB_30  */
  YYSYMBOL__SYMB_31 = 32,                  /* _SYMB_31  */
  YYSYMBOL__SYMB_32 = 33,                  /* _SYMB_32  */
  YYSYMBOL__SYMB_33 = 34,                  /* _SYMB_33  */
  YYSYMBOL__SYMB_34 = 35,                  /* _SYMB_34  */
  YYSYMBOL__SYMB_35 = 36,                  /* _SYMB_35  */
  YYSYMBOL__SYMB_36 = 37,                  /* _SYMB_36  */
  YYSYMBOL__SYMB_37 = 38,                  /* _SYMB_37  */
  YYSYMBOL__SYMB_38 = 39,                  /* _SYMB_38  */
  YYSYMBOL__SYMB_39 = 40,                  /* _SYMB_39  */
  YYSYMBOL__SYMB_40 = 41,                  /* _SYMB_40  */
  YYSYMBOL__SYMB_41 = 42,                  /* _SYMB_41  */
  YYSYMBOL__SYMB_42 = 43,                  /* _SYMB_42  */
  YYSYMBOL__SYMB_43 = 44,                  /* _SYMB_43  */
  YYSYMBOL__SYMB_3 = 45,                   /* _SYMB_3  */
  YYSYMBOL__SYMB_17 = 46,                  /* _SYMB_17  */
  YYSYMBOL__SYMB_4 = 47,                   /* _SYMB_4  */
  YYSYMBOL__STRING_ = 48,                  /* _STRING_  */
  YYSYMBOL__INTEGER_ = 49,                 /* _INTEGER_  */
  YYSYMBOL__IDENT_ = 50,                   /* _IDENT_  */
  YYSYMBOL_YYACCEPT = 51,                  /* $accept  */
  YYSYMBOL_Program = 52,                   /* Program  */
  YYSYMBOL_TopDef = 53,                    /* TopDef  */
  YYSYMBOL_ListTopDef = 54,                /* ListTopDef  */
  YYSYMBOL_Arg = 55,                       /* Arg  */
  YYSYMBOL_ListArg = 56,                   /* ListArg  */
  YYSYMBOL_ClassBlock = 57,                /* ClassBlock  */
  YYSYMBOL_ListClassDecl = 58,             /* ListClassDecl  */
  YYSYMBOL_ClassDecl = 59,                 /* ClassDecl  */
  YYSYMBOL_ListIdent = 60,                 /* ListIdent  */
  YYSYMBOL_Block = 61,                     /* Block  */
  YYSYMBOL_ListStmt = 62,                  /* ListStmt  */
  YYSYMBOL_Stmt = 63,                      /* Stmt  */
  YYSYMBOL_Item = 64,                      /* Item  */
  YYSYMBOL_ListItem = 65,                  /* ListItem  */
  YYSYMBOL_TypeSimple = 66,                /* TypeSimple  */
  YYSYMBOL_Type = 67,                      /* Type  */
  YYSYMBOL_FieldVal = 68,                  /* FieldVal  */
  YYSYMBOL_Field = 69,                     /* Field  */
  YYSYMBOL_ListField = 70,                 /* ListField  */
  YYSYMBOL_Expr6 = 71,                     /* Expr6  */
  YYSYMBOL_Expr5 = 72,                     /* Expr5  */
  YYSYMBOL_Expr4 = 73,                     /* Expr4  */
  YYSYMBOL_Expr3 = 74,                     /* Expr3  */
  YYSYMBOL_Expr2 = 75,                     /* Expr2  */
  YYSYMBOL_Expr1 = 76,                     /* Expr1  */
  YYSYMBOL_Expr = 77,                      /* Expr  */
  YYSYMBOL_ListExpr = 78,                  /* ListExpr  */
  YYSYMBOL_AddOp = 79,                     /* AddOp  */
  YYSYMBOL_MulOp = 80,                     /* MulOp  */
  YYSYMBOL_RelOp = 81                      /* RelOp  */
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
#define YYFINAL  13
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   234

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  51
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  31
/* YYNRULES -- Number of rules.  */
#define YYNRULES  93
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  167

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   305


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
      45,    46,    47,    48,    49,    50
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   289,   289,   291,   292,   293,   295,   296,   298,   300,
     301,   302,   304,   306,   307,   309,   310,   312,   313,   315,
     317,   318,   320,   321,   322,   323,   324,   325,   326,   327,
     328,   329,   330,   331,   332,   334,   335,   337,   338,   340,
     341,   342,   343,   345,   346,   347,   353,   354,   356,   357,
     358,   360,   361,   363,   364,   365,   366,   367,   368,   369,
     370,   371,   372,   373,   374,   375,   376,   378,   379,   380,
     382,   383,   385,   386,   388,   389,   391,   392,   394,   395,
     397,   398,   399,   401,   402,   404,   405,   406,   408,   409,
     410,   411,   412,   413
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
  "\"end of file\"", "error", "\"invalid token\"", "_ERROR_", "_SYMB_15",
  "_SYMB_27", "_SYMB_21", "_SYMB_16", "_SYMB_0", "_SYMB_1", "_SYMB_19",
  "_SYMB_18", "_SYMB_7", "_SYMB_2", "_SYMB_14", "_SYMB_8", "_SYMB_13",
  "_SYMB_20", "_SYMB_9", "_SYMB_5", "_SYMB_22", "_SYMB_23", "_SYMB_6",
  "_SYMB_26", "_SYMB_24", "_SYMB_25", "_SYMB_11", "_SYMB_10", "_SYMB_12",
  "_SYMB_28", "_SYMB_29", "_SYMB_30", "_SYMB_31", "_SYMB_32", "_SYMB_33",
  "_SYMB_34", "_SYMB_35", "_SYMB_36", "_SYMB_37", "_SYMB_38", "_SYMB_39",
  "_SYMB_40", "_SYMB_41", "_SYMB_42", "_SYMB_43", "_SYMB_3", "_SYMB_17",
  "_SYMB_4", "_STRING_", "_INTEGER_", "_IDENT_", "$accept", "Program",
  "TopDef", "ListTopDef", "Arg", "ListArg", "ClassBlock", "ListClassDecl",
  "ClassDecl", "ListIdent", "Block", "ListStmt", "Stmt", "Item",
  "ListItem", "TypeSimple", "Type", "FieldVal", "Field", "ListField",
  "Expr6", "Expr5", "Expr4", "Expr3", "Expr2", "Expr1", "Expr", "ListExpr",
  "AddOp", "MulOp", "RelOp", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305
};
#endif

#define YYPACT_NINF (-92)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-43)

#define yytable_value_is_error(Yyn) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      73,   -92,   -38,   -92,   -92,   -92,   -92,    13,    73,   -92,
      -8,   -17,   -27,   -92,   -92,   -92,    32,    -7,   -92,   -92,
      -6,     5,   174,    41,    55,    15,   -92,   -92,   -92,    24,
      -6,    30,   -92,    64,    60,   -92,   -92,   -92,    -6,    48,
     -92,    47,    92,    93,   -92,   159,   114,   159,   -92,   -92,
      99,   111,   -19,   -92,    20,   -92,   -92,   112,   -92,   -92,
     -92,    -2,   -92,   -92,    71,    98,   126,   -92,   -92,   -92,
      10,    18,   206,    81,    37,    30,   140,   -92,   130,   152,
     -92,    76,   145,   139,   143,   -92,   147,   145,   145,   148,
     158,   153,   145,   -29,   -92,   -92,   -92,   145,   -92,   -92,
     145,   -92,   145,   -92,   -92,   -92,   -92,   -92,   145,   145,
     154,   155,   -92,   145,   -92,   166,   -12,   131,   171,   145,
     -92,   175,   173,   180,   145,    71,   -92,   163,   -92,   -92,
      10,   -92,    18,   -92,   -92,   -92,   181,   164,   -29,   145,
     -92,   186,    96,   177,    96,   145,   -92,   -92,   -92,   -92,
     -92,   -92,   -92,   184,   145,   183,   -92,   -92,   -92,   200,
     209,    96,   -29,    96,   -92,   -92,   -92
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,    41,     0,    39,    40,    43,    42,     0,     6,     2,
      44,     0,     0,     1,     7,    45,     0,     0,    13,     4,
       9,     0,     0,    10,     0,     0,     5,    12,    14,     0,
       9,     0,     8,    17,     0,    11,    20,     3,     9,     0,
      15,     0,     0,    17,    18,     0,     0,     0,    22,    64,
       0,     0,     0,    60,     0,    50,    63,     0,    19,    65,
      62,    47,    23,    21,     0,    48,    51,    61,    69,    71,
      73,    75,    77,    79,     0,     0,    47,    68,     0,     0,
      67,     0,     0,    53,     0,    29,     0,     0,    80,    35,
      37,     0,     0,     0,    87,    85,    86,     0,    83,    84,
       0,    93,     0,    88,    89,    92,    90,    91,     0,     0,
       0,     0,    34,     0,    16,     0,    66,     0,     0,     0,
      28,     0,    81,     0,     0,     0,    24,     0,    52,    70,
      72,    76,    74,    78,    26,    27,     0,     0,     0,     0,
      58,     0,     0,     0,     0,    80,    46,    36,    38,    49,
      25,    59,    55,     0,     0,    30,    54,    32,    82,    56,
       0,     0,     0,     0,    31,    57,    33
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -92,   -92,   -92,   211,   -92,   -23,   199,   -92,   -92,   189,
     -30,   -92,    16,   -92,    97,   -43,   168,   -92,   -92,   -91,
      66,   128,   123,   124,   -92,   132,   -46,    88,   -92,   -92,
     -92
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
      -1,     7,     8,     9,    23,    24,    19,    22,    28,    34,
      62,    41,    63,    90,    91,    10,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,   123,   100,    97,
     108
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      79,    37,   128,    78,   138,    17,    88,    35,    86,    84,
       1,    55,    12,    13,   139,    42,    94,     3,    18,    15,
      95,    76,     4,     1,    45,   -42,   140,    96,    46,    98,
       3,    83,    99,    16,    47,     4,   118,     5,   117,    85,
      20,   121,   122,    21,     6,   114,   127,   152,   -42,   110,
      18,    45,   111,    49,    30,    46,   112,    52,    53,   113,
      55,    47,    56,   133,    31,    32,    48,   136,    59,    60,
      76,   165,    38,   143,    33,    36,     1,    39,   147,    40,
      49,    50,    51,     3,    52,    53,    54,    55,     4,    56,
       5,    57,    36,   153,    58,    59,    60,    61,    43,   122,
      45,    75,     1,     2,    46,     1,    39,    81,   160,     3,
      47,    77,     3,    80,     4,    48,     5,     4,    45,    82,
      87,    89,    46,     6,    92,     1,     6,   109,    47,    49,
      50,    51,     3,    52,    53,    54,    55,     4,    56,     5,
      57,    36,    93,     1,    59,    60,    61,    49,    88,    45,
       3,    52,    53,    46,    55,     4,    56,   115,   155,    47,
     157,   116,    59,    60,    61,   -42,   120,    46,    11,   119,
     124,   125,   126,   134,   135,   137,    11,   164,    49,   166,
     142,   141,    52,    53,   144,    55,   145,    56,    25,   146,
      29,   149,    49,    59,    60,    76,    52,    53,    25,    55,
     150,    56,   151,     1,   154,   156,    25,    59,    60,    76,
       3,   101,   159,   102,   161,     4,   162,     5,   163,    14,
      26,    27,   148,   130,     6,   129,   103,   104,    44,   105,
     106,   107,   132,   158,   131
};

static const yytype_uint8 yycheck[] =
{
      46,    31,    93,    46,    16,    32,     8,    30,    54,    52,
      29,    40,    50,     0,    26,    38,     6,    36,    45,    27,
      10,    50,    41,    29,     4,    27,    38,    17,     8,    11,
      36,    50,    14,    50,    14,    41,    82,    43,    81,    19,
       8,    87,    88,    50,    50,    75,    92,   138,    50,    12,
      45,     4,    15,    33,    13,     8,    19,    37,    38,    22,
      40,    14,    42,   109,     9,    50,    19,   113,    48,    49,
      50,   162,     8,   119,    50,    45,    29,    13,   124,    19,
      33,    34,    35,    36,    37,    38,    39,    40,    41,    42,
      43,    44,    45,   139,    47,    48,    49,    50,    50,   145,
       4,     9,    29,    30,     8,    29,    13,     8,   154,    36,
      14,    45,    36,    47,    41,    19,    43,    41,     4,     8,
       8,    50,     8,    50,    26,    29,    50,    46,    14,    33,
      34,    35,    36,    37,    38,    39,    40,    41,    42,    43,
      44,    45,    16,    29,    48,    49,    50,    33,     8,     4,
      36,    37,    38,     8,    40,    41,    42,    27,   142,    14,
     144,     9,    48,    49,    50,    26,    19,     8,     0,    26,
      22,    13,    19,    19,    19,     9,     8,   161,    33,   163,
       9,    50,    37,    38,     9,    40,    13,    42,    20,     9,
      22,    28,    33,    48,    49,    50,    37,    38,    30,    40,
      19,    42,    38,    29,    18,    28,    38,    48,    49,    50,
      36,     5,    28,     7,    31,    41,    16,    43,     9,     8,
      21,    47,   125,   100,    50,    97,    20,    21,    39,    23,
      24,    25,   108,   145,   102
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    29,    30,    36,    41,    43,    50,    52,    53,    54,
      66,    67,    50,     0,    54,    27,    50,    32,    45,    57,
       8,    50,    58,    55,    56,    67,    57,    47,    59,    67,
      13,     9,    50,    50,    60,    56,    45,    61,     8,    13,
      19,    62,    56,    50,    60,     4,     8,    14,    19,    33,
      34,    35,    37,    38,    39,    40,    42,    44,    47,    48,
      49,    50,    61,    63,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,     9,    50,    71,    66,    77,
      71,     8,     8,    50,    66,    19,    77,     8,     8,    50,
      64,    65,    26,    16,     6,    10,    17,    80,    11,    14,
      79,     5,     7,    20,    21,    23,    24,    25,    81,    46,
      12,    15,    19,    22,    61,    27,     9,    66,    77,    26,
      19,    77,    77,    78,    22,    13,    19,    77,    70,    72,
      73,    76,    74,    77,    19,    19,    77,     9,    16,    26,
      38,    50,     9,    77,     9,    13,     9,    77,    65,    28,
      19,    38,    70,    77,    18,    63,    28,    63,    78,    28,
      77,    31,    16,     9,    63,    70,    63
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_int8 yyr1[] =
{
       0,    51,    52,    53,    53,    53,    54,    54,    55,    56,
      56,    56,    57,    58,    58,    59,    59,    60,    60,    61,
      62,    62,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    64,    64,    65,    65,    66,
      66,    66,    66,    67,    67,    67,    68,    68,    69,    69,
      69,    70,    70,    71,    71,    71,    71,    71,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    72,    72,    72,
      73,    73,    74,    74,    75,    75,    76,    76,    77,    77,
      78,    78,    78,    79,    79,    80,    80,    80,    81,    81,
      81,    81,    81,    81
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     6,     3,     5,     1,     2,     2,     0,
       1,     3,     3,     0,     2,     3,     6,     1,     3,     3,
       0,     2,     1,     1,     3,     4,     3,     3,     3,     2,
       5,     7,     5,     8,     2,     1,     3,     1,     3,     1,
       1,     1,     1,     1,     1,     2,     4,     1,     1,     4,
       1,     1,     3,     2,     5,     5,     6,     8,     4,     5,
       1,     1,     1,     1,     1,     1,     3,     2,     2,     1,
       3,     1,     3,     1,     3,     1,     3,     1,     3,     1,
       0,     1,     3,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1
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
#line 289 "latte.y"
                     { (yyval.program_) = make_Prog((yyvsp[0].listtopdef_)); (yyval.program_)->line_number = (yyloc).first_line; (yyval.program_)->char_number = (yyloc).first_column; YY_RESULT_Program_= (yyval.program_); }
#line 1677 "Parser.c"
    break;

  case 3: /* TopDef: Type _IDENT_ _SYMB_0 ListArg _SYMB_1 Block  */
#line 291 "latte.y"
                                                    { (yyval.topdef_) = make_FnDef((yyvsp[-5].type_), (yyvsp[-4]._string), (yyvsp[-2].listarg_), (yyvsp[0].block_)); (yyval.topdef_)->line_number = (yyloc).first_line; (yyval.topdef_)->char_number = (yyloc).first_column;  }
#line 1683 "Parser.c"
    break;

  case 4: /* TopDef: _SYMB_29 _IDENT_ ClassBlock  */
#line 292 "latte.y"
                                { (yyval.topdef_) = make_ClassDef((yyvsp[-1]._string), (yyvsp[0].classblock_)); (yyval.topdef_)->line_number = (yyloc).first_line; (yyval.topdef_)->char_number = (yyloc).first_column;  }
#line 1689 "Parser.c"
    break;

  case 5: /* TopDef: _SYMB_29 _IDENT_ _SYMB_31 _IDENT_ ClassBlock  */
#line 293 "latte.y"
                                                 { (yyval.topdef_) = make_ClassDefExt((yyvsp[-3]._string), (yyvsp[-1]._string), (yyvsp[0].classblock_)); (yyval.topdef_)->line_number = (yyloc).first_line; (yyval.topdef_)->char_number = (yyloc).first_column;  }
#line 1695 "Parser.c"
    break;

  case 6: /* ListTopDef: TopDef  */
#line 295 "latte.y"
                    { (yyval.listtopdef_) = make_ListTopDef((yyvsp[0].topdef_), 0);  }
#line 1701 "Parser.c"
    break;

  case 7: /* ListTopDef: TopDef ListTopDef  */
#line 296 "latte.y"
                      { (yyval.listtopdef_) = make_ListTopDef((yyvsp[-1].topdef_), (yyvsp[0].listtopdef_));  }
#line 1707 "Parser.c"
    break;

  case 8: /* Arg: Type _IDENT_  */
#line 298 "latte.y"
                   { (yyval.arg_) = make_Ar((yyvsp[-1].type_), (yyvsp[0]._string)); (yyval.arg_)->line_number = (yyloc).first_line; (yyval.arg_)->char_number = (yyloc).first_column;  }
#line 1713 "Parser.c"
    break;

  case 9: /* ListArg: %empty  */
#line 300 "latte.y"
                      { (yyval.listarg_) = 0;  }
#line 1719 "Parser.c"
    break;

  case 10: /* ListArg: Arg  */
#line 301 "latte.y"
        { (yyval.listarg_) = make_ListArg((yyvsp[0].arg_), 0);  }
#line 1725 "Parser.c"
    break;

  case 11: /* ListArg: Arg _SYMB_2 ListArg  */
#line 302 "latte.y"
                        { (yyval.listarg_) = make_ListArg((yyvsp[-2].arg_), (yyvsp[0].listarg_));  }
#line 1731 "Parser.c"
    break;

  case 12: /* ClassBlock: _SYMB_3 ListClassDecl _SYMB_4  */
#line 304 "latte.y"
                                           { (yyval.classblock_) = make_ClassBlk(reverseListClassDecl((yyvsp[-1].listclassdecl_))); (yyval.classblock_)->line_number = (yyloc).first_line; (yyval.classblock_)->char_number = (yyloc).first_column;  }
#line 1737 "Parser.c"
    break;

  case 13: /* ListClassDecl: %empty  */
#line 306 "latte.y"
                            { (yyval.listclassdecl_) = 0;  }
#line 1743 "Parser.c"
    break;

  case 14: /* ListClassDecl: ListClassDecl ClassDecl  */
#line 307 "latte.y"
                            { (yyval.listclassdecl_) = make_ListClassDecl((yyvsp[0].classdecl_), (yyvsp[-1].listclassdecl_));  }
#line 1749 "Parser.c"
    break;

  case 15: /* ClassDecl: Type ListIdent _SYMB_5  */
#line 309 "latte.y"
                                   { (yyval.classdecl_) = make_FieldDecl((yyvsp[-2].type_), (yyvsp[-1].listident_)); (yyval.classdecl_)->line_number = (yyloc).first_line; (yyval.classdecl_)->char_number = (yyloc).first_column;  }
#line 1755 "Parser.c"
    break;

  case 16: /* ClassDecl: Type _IDENT_ _SYMB_0 ListArg _SYMB_1 Block  */
#line 310 "latte.y"
                                               { (yyval.classdecl_) = make_MethodDecl((yyvsp[-5].type_), (yyvsp[-4]._string), (yyvsp[-2].listarg_), (yyvsp[0].block_)); (yyval.classdecl_)->line_number = (yyloc).first_line; (yyval.classdecl_)->char_number = (yyloc).first_column;  }
#line 1761 "Parser.c"
    break;

  case 17: /* ListIdent: _IDENT_  */
#line 312 "latte.y"
                    { (yyval.listident_) = make_ListIdent((yyvsp[0]._string), 0);  }
#line 1767 "Parser.c"
    break;

  case 18: /* ListIdent: _IDENT_ _SYMB_2 ListIdent  */
#line 313 "latte.y"
                              { (yyval.listident_) = make_ListIdent((yyvsp[-2]._string), (yyvsp[0].listident_));  }
#line 1773 "Parser.c"
    break;

  case 19: /* Block: _SYMB_3 ListStmt _SYMB_4  */
#line 315 "latte.y"
                                 { (yyval.block_) = make_Blk(reverseListStmt((yyvsp[-1].liststmt_))); (yyval.block_)->line_number = (yyloc).first_line; (yyval.block_)->char_number = (yyloc).first_column;  }
#line 1779 "Parser.c"
    break;

  case 20: /* ListStmt: %empty  */
#line 317 "latte.y"
                       { (yyval.liststmt_) = 0;  }
#line 1785 "Parser.c"
    break;

  case 21: /* ListStmt: ListStmt Stmt  */
#line 318 "latte.y"
                  { (yyval.liststmt_) = make_ListStmt((yyvsp[0].stmt_), (yyvsp[-1].liststmt_));  }
#line 1791 "Parser.c"
    break;

  case 22: /* Stmt: _SYMB_5  */
#line 320 "latte.y"
               { (yyval.stmt_) = make_Empty(); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1797 "Parser.c"
    break;

  case 23: /* Stmt: Block  */
#line 321 "latte.y"
          { (yyval.stmt_) = make_BStmt((yyvsp[0].block_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1803 "Parser.c"
    break;

  case 24: /* Stmt: Type ListItem _SYMB_5  */
#line 322 "latte.y"
                          { (yyval.stmt_) = make_Decl((yyvsp[-2].type_), (yyvsp[-1].listitem_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1809 "Parser.c"
    break;

  case 25: /* Stmt: Expr _SYMB_6 Expr _SYMB_5  */
#line 323 "latte.y"
                              { (yyval.stmt_) = make_Ass((yyvsp[-3].expr_), (yyvsp[-1].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1815 "Parser.c"
    break;

  case 26: /* Stmt: Expr _SYMB_7 _SYMB_5  */
#line 324 "latte.y"
                         { (yyval.stmt_) = make_Incr((yyvsp[-2].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1821 "Parser.c"
    break;

  case 27: /* Stmt: Expr _SYMB_8 _SYMB_5  */
#line 325 "latte.y"
                         { (yyval.stmt_) = make_Decr((yyvsp[-2].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1827 "Parser.c"
    break;

  case 28: /* Stmt: _SYMB_38 Expr _SYMB_5  */
#line 326 "latte.y"
                          { (yyval.stmt_) = make_Ret((yyvsp[-1].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1833 "Parser.c"
    break;

  case 29: /* Stmt: _SYMB_38 _SYMB_5  */
#line 327 "latte.y"
                     { (yyval.stmt_) = make_VRet(); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1839 "Parser.c"
    break;

  case 30: /* Stmt: _SYMB_34 _SYMB_0 Expr _SYMB_1 Stmt  */
#line 328 "latte.y"
                                       { (yyval.stmt_) = make_Cond((yyvsp[-2].expr_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1845 "Parser.c"
    break;

  case 31: /* Stmt: _SYMB_34 _SYMB_0 Expr _SYMB_1 Stmt _SYMB_30 Stmt  */
#line 329 "latte.y"
                                                     { (yyval.stmt_) = make_CondElse((yyvsp[-4].expr_), (yyvsp[-2].stmt_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1851 "Parser.c"
    break;

  case 32: /* Stmt: _SYMB_43 _SYMB_0 Expr _SYMB_1 Stmt  */
#line 330 "latte.y"
                                       { (yyval.stmt_) = make_While((yyvsp[-2].expr_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1857 "Parser.c"
    break;

  case 33: /* Stmt: _SYMB_33 _SYMB_0 TypeSimple _IDENT_ _SYMB_9 Expr _SYMB_1 Stmt  */
#line 331 "latte.y"
                                                                  { (yyval.stmt_) = make_For((yyvsp[-5].typesimple_), (yyvsp[-4]._string), (yyvsp[-2].expr_), (yyvsp[0].stmt_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1863 "Parser.c"
    break;

  case 34: /* Stmt: Expr _SYMB_5  */
#line 332 "latte.y"
                 { (yyval.stmt_) = make_SExp((yyvsp[-1].expr_)); (yyval.stmt_)->line_number = (yyloc).first_line; (yyval.stmt_)->char_number = (yyloc).first_column;  }
#line 1869 "Parser.c"
    break;

  case 35: /* Item: _IDENT_  */
#line 334 "latte.y"
               { (yyval.item_) = make_NoInit((yyvsp[0]._string)); (yyval.item_)->line_number = (yyloc).first_line; (yyval.item_)->char_number = (yyloc).first_column;  }
#line 1875 "Parser.c"
    break;

  case 36: /* Item: _IDENT_ _SYMB_6 Expr  */
#line 335 "latte.y"
                         { (yyval.item_) = make_Init((yyvsp[-2]._string), (yyvsp[0].expr_)); (yyval.item_)->line_number = (yyloc).first_line; (yyval.item_)->char_number = (yyloc).first_column;  }
#line 1881 "Parser.c"
    break;

  case 37: /* ListItem: Item  */
#line 337 "latte.y"
                { (yyval.listitem_) = make_ListItem((yyvsp[0].item_), 0);  }
#line 1887 "Parser.c"
    break;

  case 38: /* ListItem: Item _SYMB_2 ListItem  */
#line 338 "latte.y"
                          { (yyval.listitem_) = make_ListItem((yyvsp[-2].item_), (yyvsp[0].listitem_));  }
#line 1893 "Parser.c"
    break;

  case 39: /* TypeSimple: _SYMB_35  */
#line 340 "latte.y"
                      { (yyval.typesimple_) = make_Int(); (yyval.typesimple_)->line_number = (yyloc).first_line; (yyval.typesimple_)->char_number = (yyloc).first_column;  }
#line 1899 "Parser.c"
    break;

  case 40: /* TypeSimple: _SYMB_40  */
#line 341 "latte.y"
             { (yyval.typesimple_) = make_Str(); (yyval.typesimple_)->line_number = (yyloc).first_line; (yyval.typesimple_)->char_number = (yyloc).first_column;  }
#line 1905 "Parser.c"
    break;

  case 41: /* TypeSimple: _SYMB_28  */
#line 342 "latte.y"
             { (yyval.typesimple_) = make_Bool(); (yyval.typesimple_)->line_number = (yyloc).first_line; (yyval.typesimple_)->char_number = (yyloc).first_column;  }
#line 1911 "Parser.c"
    break;

  case 42: /* TypeSimple: _IDENT_  */
#line 343 "latte.y"
            { (yyval.typesimple_) = make_Class((yyvsp[0]._string)); (yyval.typesimple_)->line_number = (yyloc).first_line; (yyval.typesimple_)->char_number = (yyloc).first_column;  }
#line 1917 "Parser.c"
    break;

  case 43: /* Type: _SYMB_42  */
#line 345 "latte.y"
                { (yyval.type_) = make_Void(); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1923 "Parser.c"
    break;

  case 44: /* Type: TypeSimple  */
#line 346 "latte.y"
               { (yyval.type_) = make_SingleType((yyvsp[0].typesimple_)); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1929 "Parser.c"
    break;

  case 45: /* Type: TypeSimple _SYMB_10  */
#line 347 "latte.y"
                        { (yyval.type_) = make_ArrayType((yyvsp[-1].typesimple_)); (yyval.type_)->line_number = (yyloc).first_line; (yyval.type_)->char_number = (yyloc).first_column;  }
#line 1935 "Parser.c"
    break;

  case 46: /* FieldVal: _IDENT_ _SYMB_0 ListExpr _SYMB_1  */
#line 353 "latte.y"
                                            { (yyval.fieldval_) = make_FieldValCall((yyvsp[-3]._string), (yyvsp[-1].listexpr_)); (yyval.fieldval_)->line_number = (yyloc).first_line; (yyval.fieldval_)->char_number = (yyloc).first_column;  }
#line 1941 "Parser.c"
    break;

  case 47: /* FieldVal: _IDENT_  */
#line 354 "latte.y"
            { (yyval.fieldval_) = make_FieldValSingle((yyvsp[0]._string)); (yyval.fieldval_)->line_number = (yyloc).first_line; (yyval.fieldval_)->char_number = (yyloc).first_column;  }
#line 1947 "Parser.c"
    break;

  case 48: /* Field: FieldVal  */
#line 356 "latte.y"
                 { (yyval.field_) = make_FieldSingle((yyvsp[0].fieldval_)); (yyval.field_)->line_number = (yyloc).first_line; (yyval.field_)->char_number = (yyloc).first_column;  }
#line 1953 "Parser.c"
    break;

  case 49: /* Field: FieldVal _SYMB_11 Expr _SYMB_12  */
#line 357 "latte.y"
                                    { (yyval.field_) = make_FieldArray((yyvsp[-3].fieldval_), (yyvsp[-1].expr_)); (yyval.field_)->line_number = (yyloc).first_line; (yyval.field_)->char_number = (yyloc).first_column;  }
#line 1959 "Parser.c"
    break;

  case 50: /* Field: _SYMB_39  */
#line 358 "latte.y"
             { (yyval.field_) = make_FieldSelf(); (yyval.field_)->line_number = (yyloc).first_line; (yyval.field_)->char_number = (yyloc).first_column;  }
#line 1965 "Parser.c"
    break;

  case 51: /* ListField: Field  */
#line 360 "latte.y"
                  { (yyval.listfield_) = make_ListField((yyvsp[0].field_), 0);  }
#line 1971 "Parser.c"
    break;

  case 52: /* ListField: Field _SYMB_13 ListField  */
#line 361 "latte.y"
                             { (yyval.listfield_) = make_ListField((yyvsp[-2].field_), (yyvsp[0].listfield_));  }
#line 1977 "Parser.c"
    break;

  case 53: /* Expr6: _SYMB_36 _IDENT_  */
#line 363 "latte.y"
                         { (yyval.expr_) = make_ENewObject((yyvsp[0]._string)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1983 "Parser.c"
    break;

  case 54: /* Expr6: _SYMB_36 TypeSimple _SYMB_11 Expr _SYMB_12  */
#line 364 "latte.y"
                                               { (yyval.expr_) = make_ENewArray((yyvsp[-3].typesimple_), (yyvsp[-1].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1989 "Parser.c"
    break;

  case 55: /* Expr6: _SYMB_0 Expr _SYMB_1 _SYMB_13 ListField  */
#line 365 "latte.y"
                                            { (yyval.expr_) = make_ETmpVar((yyvsp[-3].expr_), (yyvsp[0].listfield_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 1995 "Parser.c"
    break;

  case 56: /* Expr6: _SYMB_0 Expr _SYMB_1 _SYMB_11 Expr _SYMB_12  */
#line 366 "latte.y"
                                                { (yyval.expr_) = make_ETmpArrayElem((yyvsp[-4].expr_), (yyvsp[-1].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2001 "Parser.c"
    break;

  case 57: /* Expr6: _SYMB_0 Expr _SYMB_1 _SYMB_11 Expr _SYMB_12 _SYMB_13 ListField  */
#line 367 "latte.y"
                                                                   { (yyval.expr_) = make_ETmpArrayElemVar((yyvsp[-6].expr_), (yyvsp[-3].expr_), (yyvsp[0].listfield_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2007 "Parser.c"
    break;

  case 58: /* Expr6: _SYMB_0 Expr _SYMB_1 _SYMB_37  */
#line 368 "latte.y"
                                  { (yyval.expr_) = make_ECastedNull((yyvsp[-2].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2013 "Parser.c"
    break;

  case 59: /* Expr6: _SYMB_0 TypeSimple _SYMB_10 _SYMB_1 _SYMB_37  */
#line 369 "latte.y"
                                                 { (yyval.expr_) = make_ECastedArrNull((yyvsp[-3].typesimple_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2019 "Parser.c"
    break;

  case 60: /* Expr6: _SYMB_37  */
#line 370 "latte.y"
             { (yyval.expr_) = make_ENull(); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2025 "Parser.c"
    break;

  case 61: /* Expr6: ListField  */
#line 371 "latte.y"
              { (yyval.expr_) = make_EVar((yyvsp[0].listfield_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2031 "Parser.c"
    break;

  case 62: /* Expr6: _INTEGER_  */
#line 372 "latte.y"
              { (yyval.expr_) = make_ELitInt((yyvsp[0]._int)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2037 "Parser.c"
    break;

  case 63: /* Expr6: _SYMB_41  */
#line 373 "latte.y"
             { (yyval.expr_) = make_ELitTrue(); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2043 "Parser.c"
    break;

  case 64: /* Expr6: _SYMB_32  */
#line 374 "latte.y"
             { (yyval.expr_) = make_ELitFalse(); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2049 "Parser.c"
    break;

  case 65: /* Expr6: _STRING_  */
#line 375 "latte.y"
             { (yyval.expr_) = make_EString((yyvsp[0]._string)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2055 "Parser.c"
    break;

  case 66: /* Expr6: _SYMB_0 Expr _SYMB_1  */
#line 376 "latte.y"
                         { (yyval.expr_) = (yyvsp[-1].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2061 "Parser.c"
    break;

  case 67: /* Expr5: _SYMB_14 Expr6  */
#line 378 "latte.y"
                       { (yyval.expr_) = make_Neg((yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2067 "Parser.c"
    break;

  case 68: /* Expr5: _SYMB_15 Expr6  */
#line 379 "latte.y"
                   { (yyval.expr_) = make_Not((yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2073 "Parser.c"
    break;

  case 69: /* Expr5: Expr6  */
#line 380 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2079 "Parser.c"
    break;

  case 70: /* Expr4: Expr4 MulOp Expr5  */
#line 382 "latte.y"
                          { (yyval.expr_) = make_EMul((yyvsp[-2].expr_), (yyvsp[-1].mulop_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2085 "Parser.c"
    break;

  case 71: /* Expr4: Expr5  */
#line 383 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2091 "Parser.c"
    break;

  case 72: /* Expr3: Expr3 AddOp Expr4  */
#line 385 "latte.y"
                          { (yyval.expr_) = make_EAdd((yyvsp[-2].expr_), (yyvsp[-1].addop_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2097 "Parser.c"
    break;

  case 73: /* Expr3: Expr4  */
#line 386 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2103 "Parser.c"
    break;

  case 74: /* Expr2: Expr2 RelOp Expr3  */
#line 388 "latte.y"
                          { (yyval.expr_) = make_ERel((yyvsp[-2].expr_), (yyvsp[-1].relop_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2109 "Parser.c"
    break;

  case 75: /* Expr2: Expr3  */
#line 389 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2115 "Parser.c"
    break;

  case 76: /* Expr1: Expr2 _SYMB_16 Expr1  */
#line 391 "latte.y"
                             { (yyval.expr_) = make_EAnd((yyvsp[-2].expr_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2121 "Parser.c"
    break;

  case 77: /* Expr1: Expr2  */
#line 392 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2127 "Parser.c"
    break;

  case 78: /* Expr: Expr1 _SYMB_17 Expr  */
#line 394 "latte.y"
                           { (yyval.expr_) = make_EOr((yyvsp[-2].expr_), (yyvsp[0].expr_)); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2133 "Parser.c"
    break;

  case 79: /* Expr: Expr1  */
#line 395 "latte.y"
          { (yyval.expr_) = (yyvsp[0].expr_); (yyval.expr_)->line_number = (yyloc).first_line; (yyval.expr_)->char_number = (yyloc).first_column;  }
#line 2139 "Parser.c"
    break;

  case 80: /* ListExpr: %empty  */
#line 397 "latte.y"
                       { (yyval.listexpr_) = 0;  }
#line 2145 "Parser.c"
    break;

  case 81: /* ListExpr: Expr  */
#line 398 "latte.y"
         { (yyval.listexpr_) = make_ListExpr((yyvsp[0].expr_), 0);  }
#line 2151 "Parser.c"
    break;

  case 82: /* ListExpr: Expr _SYMB_2 ListExpr  */
#line 399 "latte.y"
                          { (yyval.listexpr_) = make_ListExpr((yyvsp[-2].expr_), (yyvsp[0].listexpr_));  }
#line 2157 "Parser.c"
    break;

  case 83: /* AddOp: _SYMB_18  */
#line 401 "latte.y"
                 { (yyval.addop_) = make_Plus(); (yyval.addop_)->line_number = (yyloc).first_line; (yyval.addop_)->char_number = (yyloc).first_column;  }
#line 2163 "Parser.c"
    break;

  case 84: /* AddOp: _SYMB_14  */
#line 402 "latte.y"
             { (yyval.addop_) = make_Minus(); (yyval.addop_)->line_number = (yyloc).first_line; (yyval.addop_)->char_number = (yyloc).first_column;  }
#line 2169 "Parser.c"
    break;

  case 85: /* MulOp: _SYMB_19  */
#line 404 "latte.y"
                 { (yyval.mulop_) = make_Times(); (yyval.mulop_)->line_number = (yyloc).first_line; (yyval.mulop_)->char_number = (yyloc).first_column;  }
#line 2175 "Parser.c"
    break;

  case 86: /* MulOp: _SYMB_20  */
#line 405 "latte.y"
             { (yyval.mulop_) = make_Div(); (yyval.mulop_)->line_number = (yyloc).first_line; (yyval.mulop_)->char_number = (yyloc).first_column;  }
#line 2181 "Parser.c"
    break;

  case 87: /* MulOp: _SYMB_21  */
#line 406 "latte.y"
             { (yyval.mulop_) = make_Mod(); (yyval.mulop_)->line_number = (yyloc).first_line; (yyval.mulop_)->char_number = (yyloc).first_column;  }
#line 2187 "Parser.c"
    break;

  case 88: /* RelOp: _SYMB_22  */
#line 408 "latte.y"
                 { (yyval.relop_) = make_LTH(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 2193 "Parser.c"
    break;

  case 89: /* RelOp: _SYMB_23  */
#line 409 "latte.y"
             { (yyval.relop_) = make_LE(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 2199 "Parser.c"
    break;

  case 90: /* RelOp: _SYMB_24  */
#line 410 "latte.y"
             { (yyval.relop_) = make_GTH(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 2205 "Parser.c"
    break;

  case 91: /* RelOp: _SYMB_25  */
#line 411 "latte.y"
             { (yyval.relop_) = make_GE(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 2211 "Parser.c"
    break;

  case 92: /* RelOp: _SYMB_26  */
#line 412 "latte.y"
             { (yyval.relop_) = make_EQU(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 2217 "Parser.c"
    break;

  case 93: /* RelOp: _SYMB_27  */
#line 413 "latte.y"
             { (yyval.relop_) = make_NE(); (yyval.relop_)->line_number = (yyloc).first_line; (yyval.relop_)->char_number = (yyloc).first_column;  }
#line 2223 "Parser.c"
    break;


#line 2227 "Parser.c"

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

#line 416 "latte.y"

void __attribute__((weak)) yyerror(const char *str)
{
  extern char *lattetext;
  fprintf(stderr,"error: %d,%d: %s at %s\n",
  lattelloc.first_line, lattelloc.first_column, str, lattetext);
}

