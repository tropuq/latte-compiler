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
} YYSTYPE;

typedef struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
} YYLTYPE;

#define _ERROR_ 258
#define _SYMB_15 259
#define _SYMB_27 260
#define _SYMB_21 261
#define _SYMB_16 262
#define _SYMB_0 263
#define _SYMB_1 264
#define _SYMB_19 265
#define _SYMB_18 266
#define _SYMB_7 267
#define _SYMB_2 268
#define _SYMB_14 269
#define _SYMB_8 270
#define _SYMB_13 271
#define _SYMB_20 272
#define _SYMB_9 273
#define _SYMB_5 274
#define _SYMB_22 275
#define _SYMB_23 276
#define _SYMB_6 277
#define _SYMB_26 278
#define _SYMB_24 279
#define _SYMB_25 280
#define _SYMB_11 281
#define _SYMB_10 282
#define _SYMB_12 283
#define _SYMB_28 284
#define _SYMB_29 285
#define _SYMB_30 286
#define _SYMB_31 287
#define _SYMB_32 288
#define _SYMB_33 289
#define _SYMB_34 290
#define _SYMB_35 291
#define _SYMB_36 292
#define _SYMB_37 293
#define _SYMB_38 294
#define _SYMB_39 295
#define _SYMB_40 296
#define _SYMB_41 297
#define _SYMB_42 298
#define _SYMB_43 299
#define _SYMB_3 300
#define _SYMB_17 301
#define _SYMB_4 302
#define _STRING_ 303
#define _INTEGER_ 304
#define _IDENT_ 305


extern YYLTYPE yylloc;
extern YYSTYPE yylval;

Program  pProgram(FILE *inp);
Program psProgram(const char *str);

#endif
