#ifndef PARSER_HEADER_FILE
#define PARSER_HEADER_FILE

#include "Absyn.h"

typedef union
{
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
} YYSTYPE;

typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;

#define _ERROR_ 258
#define _SYMB_10 259
#define _SYMB_22 260
#define _SYMB_16 261
#define _SYMB_11 262
#define _SYMB_0 263
#define _SYMB_1 264
#define _SYMB_14 265
#define _SYMB_13 266
#define _SYMB_7 267
#define _SYMB_2 268
#define _SYMB_9 269
#define _SYMB_8 270
#define _SYMB_15 271
#define _SYMB_5 272
#define _SYMB_17 273
#define _SYMB_18 274
#define _SYMB_6 275
#define _SYMB_21 276
#define _SYMB_19 277
#define _SYMB_20 278
#define _SYMB_23 279
#define _SYMB_24 280
#define _SYMB_25 281
#define _SYMB_26 282
#define _SYMB_27 283
#define _SYMB_28 284
#define _SYMB_29 285
#define _SYMB_30 286
#define _SYMB_31 287
#define _SYMB_32 288
#define _SYMB_3 289
#define _SYMB_12 290
#define _SYMB_4 291
#define _STRING_ 292
#define _INTEGER_ 293
#define _IDENT_ 294


extern YYLTYPE yylloc;
extern YYSTYPE yylval;

Program  pProgram(FILE *inp);
Program psProgram(const char *str);

#endif
