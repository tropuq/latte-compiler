/* This Bison file was machine-generated by BNFC */
%locations
%{
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


%}

%union
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
}

%token _ERROR_
%token _SYMB_15    /*   !   */
%token _SYMB_27    /*   !=   */
%token _SYMB_21    /*   %   */
%token _SYMB_16    /*   &&   */
%token _SYMB_0    /*   (   */
%token _SYMB_1    /*   )   */
%token _SYMB_19    /*   *   */
%token _SYMB_18    /*   +   */
%token _SYMB_7    /*   ++   */
%token _SYMB_2    /*   ,   */
%token _SYMB_14    /*   -   */
%token _SYMB_8    /*   --   */
%token _SYMB_13    /*   .   */
%token _SYMB_20    /*   /   */
%token _SYMB_9    /*   :   */
%token _SYMB_5    /*   ;   */
%token _SYMB_22    /*   <   */
%token _SYMB_23    /*   <=   */
%token _SYMB_6    /*   =   */
%token _SYMB_26    /*   ==   */
%token _SYMB_24    /*   >   */
%token _SYMB_25    /*   >=   */
%token _SYMB_11    /*   [   */
%token _SYMB_10    /*   []   */
%token _SYMB_12    /*   ]   */
%token _SYMB_28    /*   boolean   */
%token _SYMB_29    /*   class   */
%token _SYMB_30    /*   else   */
%token _SYMB_31    /*   extends   */
%token _SYMB_32    /*   false   */
%token _SYMB_33    /*   for   */
%token _SYMB_34    /*   if   */
%token _SYMB_35    /*   int   */
%token _SYMB_36    /*   new   */
%token _SYMB_37    /*   null   */
%token _SYMB_38    /*   return   */
%token _SYMB_39    /*   self   */
%token _SYMB_40    /*   string   */
%token _SYMB_41    /*   true   */
%token _SYMB_42    /*   void   */
%token _SYMB_43    /*   while   */
%token _SYMB_3    /*   {   */
%token _SYMB_17    /*   ||   */
%token _SYMB_4    /*   }   */

%type <program_> Program
%type <topdef_> TopDef
%type <listtopdef_> ListTopDef
%type <arg_> Arg
%type <listarg_> ListArg
%type <classblock_> ClassBlock
%type <listclassdecl_> ListClassDecl
%type <classdecl_> ClassDecl
%type <listident_> ListIdent
%type <block_> Block
%type <liststmt_> ListStmt
%type <stmt_> Stmt
%type <item_> Item
%type <listitem_> ListItem
%type <typesimple_> TypeSimple
%type <type_> Type
%type <listtype_> ListType
%type <fieldval_> FieldVal
%type <field_> Field
%type <listfield_> ListField
%type <expr_> Expr6
%type <expr_> Expr5
%type <expr_> Expr4
%type <expr_> Expr3
%type <expr_> Expr2
%type <expr_> Expr1
%type <expr_> Expr
%type <listexpr_> ListExpr
%type <addop_> AddOp
%type <mulop_> MulOp
%type <relop_> RelOp

%token<_string> _STRING_
%token<_int>    _INTEGER_
%token<_string> _IDENT_

%start Program
%%
Program : ListTopDef { $$ = make_Prog($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column; YY_RESULT_Program_= $$; }
;
TopDef : Type _IDENT_ _SYMB_0 ListArg _SYMB_1 Block { $$ = make_FnDef($1, $2, $4, $6); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_29 _IDENT_ ClassBlock { $$ = make_ClassDef($2, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_29 _IDENT_ _SYMB_31 _IDENT_ ClassBlock { $$ = make_ClassDefExt($2, $4, $5); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListTopDef : TopDef { $$ = make_ListTopDef($1, 0);  }
  | TopDef ListTopDef { $$ = make_ListTopDef($1, $2);  }
;
Arg : Type _IDENT_ { $$ = make_Ar($1, $2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListArg : /* empty */ { $$ = 0;  }
  | Arg { $$ = make_ListArg($1, 0);  }
  | Arg _SYMB_2 ListArg { $$ = make_ListArg($1, $3);  }
;
ClassBlock : _SYMB_3 ListClassDecl _SYMB_4 { $$ = make_ClassBlk(reverseListClassDecl($2)); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListClassDecl : /* empty */ { $$ = 0;  }
  | ListClassDecl ClassDecl { $$ = make_ListClassDecl($2, $1);  }
;
ClassDecl : Type ListIdent _SYMB_5 { $$ = make_FieldDecl($1, $2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Type _IDENT_ _SYMB_0 ListArg _SYMB_1 Block { $$ = make_MethodDecl($1, $2, $4, $6); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListIdent : _IDENT_ { $$ = make_ListIdent($1, 0);  }
  | _IDENT_ _SYMB_2 ListIdent { $$ = make_ListIdent($1, $3);  }
;
Block : _SYMB_3 ListStmt _SYMB_4 { $$ = make_Blk(reverseListStmt($2)); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListStmt : /* empty */ { $$ = 0;  }
  | ListStmt Stmt { $$ = make_ListStmt($2, $1);  }
;
Stmt : _SYMB_5 { $$ = make_Empty(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Block { $$ = make_BStmt($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Type ListItem _SYMB_5 { $$ = make_Decl($1, $2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr _SYMB_6 Expr _SYMB_5 { $$ = make_Ass($1, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr _SYMB_7 _SYMB_5 { $$ = make_Incr($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr _SYMB_8 _SYMB_5 { $$ = make_Decr($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_38 Expr _SYMB_5 { $$ = make_Ret($2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_38 _SYMB_5 { $$ = make_VRet(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_34 _SYMB_0 Expr _SYMB_1 Stmt { $$ = make_Cond($3, $5); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_34 _SYMB_0 Expr _SYMB_1 Stmt _SYMB_30 Stmt { $$ = make_CondElse($3, $5, $7); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_43 _SYMB_0 Expr _SYMB_1 Stmt { $$ = make_While($3, $5); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_33 _SYMB_0 TypeSimple _IDENT_ _SYMB_9 Expr _SYMB_1 Stmt { $$ = make_For($3, $4, $6, $8); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr _SYMB_5 { $$ = make_SExp($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Item : _IDENT_ { $$ = make_NoInit($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _IDENT_ _SYMB_6 Expr { $$ = make_Init($1, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListItem : Item { $$ = make_ListItem($1, 0);  }
  | Item _SYMB_2 ListItem { $$ = make_ListItem($1, $3);  }
;
TypeSimple : _SYMB_35 { $$ = make_Int(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_40 { $$ = make_Str(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_28 { $$ = make_Bool(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _IDENT_ { $$ = make_Class($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Type : _SYMB_42 { $$ = make_Void(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | TypeSimple { $$ = make_SingleType($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | TypeSimple _SYMB_10 { $$ = make_ArrayType($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListType : /* empty */ { $$ = 0;  }
  | Type { $$ = make_ListType($1, 0);  }
  | Type _SYMB_2 ListType { $$ = make_ListType($1, $3);  }
;
FieldVal : _IDENT_ _SYMB_0 ListExpr _SYMB_1 { $$ = make_FieldValCall($1, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _IDENT_ { $$ = make_FieldValSingle($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Field : FieldVal { $$ = make_FieldSingle($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | FieldVal _SYMB_11 Expr _SYMB_12 { $$ = make_FieldArray($1, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_39 { $$ = make_FieldSelf(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListField : Field { $$ = make_ListField($1, 0);  }
  | Field _SYMB_13 ListField { $$ = make_ListField($1, $3);  }
;
Expr6 : _SYMB_36 _IDENT_ { $$ = make_ENewObject($2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_36 TypeSimple _SYMB_11 Expr _SYMB_12 { $$ = make_ENewArray($2, $4); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_0 Expr _SYMB_1 _SYMB_13 ListField { $$ = make_ETmpVar($2, $5); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_0 Expr _SYMB_1 _SYMB_11 Expr _SYMB_12 { $$ = make_ETmpArrayElem($2, $5); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_0 Expr _SYMB_1 _SYMB_11 Expr _SYMB_12 _SYMB_13 ListField { $$ = make_ETmpArrayElemVar($2, $5, $8); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_0 Expr _SYMB_1 _SYMB_37 { $$ = make_ECastedNull($2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_0 TypeSimple _SYMB_10 _SYMB_1 _SYMB_37 { $$ = make_ECastedArrNull($2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | ListField { $$ = make_EVar($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _INTEGER_ { $$ = make_ELitInt($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_41 { $$ = make_ELitTrue(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_32 { $$ = make_ELitFalse(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _STRING_ { $$ = make_EString($1); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_0 Expr _SYMB_1 { $$ = $2; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Expr5 : _SYMB_14 Expr6 { $$ = make_Neg($2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_15 Expr6 { $$ = make_Not($2); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr6 { $$ = $1; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Expr4 : Expr4 MulOp Expr5 { $$ = make_EMul($1, $2, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr5 { $$ = $1; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Expr3 : Expr3 AddOp Expr4 { $$ = make_EAdd($1, $2, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr4 { $$ = $1; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Expr2 : Expr2 RelOp Expr3 { $$ = make_ERel($1, $2, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr3 { $$ = $1; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Expr1 : Expr2 _SYMB_16 Expr1 { $$ = make_EAnd($1, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr2 { $$ = $1; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
Expr : Expr1 _SYMB_17 Expr { $$ = make_EOr($1, $3); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | Expr1 { $$ = $1; $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
ListExpr : /* empty */ { $$ = 0;  }
  | Expr { $$ = make_ListExpr($1, 0);  }
  | Expr _SYMB_2 ListExpr { $$ = make_ListExpr($1, $3);  }
;
AddOp : _SYMB_18 { $$ = make_Plus(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_14 { $$ = make_Minus(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
MulOp : _SYMB_19 { $$ = make_Times(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_20 { $$ = make_Div(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_21 { $$ = make_Mod(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;
RelOp : _SYMB_22 { $$ = make_LTH(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_23 { $$ = make_LE(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_24 { $$ = make_GTH(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_25 { $$ = make_GE(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_26 { $$ = make_EQU(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
  | _SYMB_27 { $$ = make_NE(); $$->line_number = @$.first_line; $$->char_number = @$.first_column;  }
;

%%
void yyerror(const char *str)
{
  extern char *lattetext;
  fprintf(stderr,"error: %d,%d: %s at %s\n",
  lattelloc.first_line, lattelloc.first_column, str, lattetext);
}

