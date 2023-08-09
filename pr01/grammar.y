%{
#include <stdlib.h>
#include <stdio.h>
#include "symboltable.c"

// symbolTableglobal = newSymbolTable();
// symbolTablecurrent = symbolTableglobal;
extern FILE *yyin;
extern int yylex();
extern char * yytext;
char lineNum_String [50];
char colNum_String [50];
extern int lineNumber;
extern int columnNumber;
void yyerror(char* arr) {
  printf("\n%s\n", arr);
  return;
}
%}

%union{
 struct Basic *basic;
 struct ConstantValue *k;
 struct ExpressionTypeInfo *t;
 struct StatementInfo *stmt;
 struct SymbolTableEntry *symtabe;
 struct ParameterDec *Pd;
 struct SymbolTable *symtab;

}

// identifier             
%token ID          101

// TYPE names
%token T_INTEGER   201
%token T_REAL      202
%token T_BOOLEAN   203
%token T_CHARACTER 204
%token T_STRING    205

// C_INTEGERs
%token C_INTEGER   301
%token C_REAL      302
%token C_CHARACTER 303
%token C_STRING    304
%token C_TRUE      305
%token C_FALSE     306

// keywords
%token NULL_PTR    401
%token FOR         402
%token WHILE       403
%token IF          404
%token THEN        405
%token ELSE        406
%token SWITCH      407
%token CASE        408
%token OTHERWISE   409
%token TYPE        410
%token FUNCTION    411

// punctuation - grouping
%token L_PARENTHESIS       501
%token R_PARENTHESIS       502
%token L_BRACKET   503
%token R_BRACKET   504
%token L_BRACE     505
%token R_BRACE     506

// punctuation - other
%token SEMI_COLON  507
%token COLON       508
%token COMMA       509
%token ARROW       510
%token DOT         511                    

// operators
%token ADD         601
%token SUB_OR_NEG  602
%token MUL         603
%token DIV         604
%token REM         605
%token LESS_THAN   606
%token EQUAL_TO    607
%token ASSIGN      608
%token INT2REAL    609
%token REAL2INT    610
%token IS_NULL     611
%token NOT         612
%token AND         613
%token OR          614
%token RESERVE     615
%token RELEASE     616

// comments
%token COMMENT     700

%type <basic> id
%type <k> non_empty_parameter_list
%type <k> parameter_declaration
%type <basic> parameter_list
%type <basic> typeName
// %type <basic> primitiveTypeName
// %type <basic> T_INTEGER
// %type <basic> T_BOOLEAN
// %type <basic> T_REAL
// %type <basic> T_CHARACTER
// %type <basic> T_STRING
// %type <basic> parameter-list
%type <symtabe> pblock

// %type <k> C_INTEGER
// %type <k> C_REAL
// %type <k> C_CHARACTER
// %type <k> C_STRING
// %type <k> C_TRUE
// %type <k> C_FALSE
 %type <k> constant
// %type <k> initialization
%type <k> assignable
%type <k> expression
%type <k> simple_statement
%type <k> identifier_list
%type <k> definition_list
%type <k> declaration
%type <k> declaration_list
%type <k> definition
%type <k> dblock
%type <k> statement_list
%type <k> statement
%type <k> sblock
%type <basic> binaryOperator

%start program

%%

program 
: definition_list sblock {}
;

constant:
  C_INTEGER 
  {
    $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
    char * x= strdup(yytext);
    int y= atoi(x);
    $$->type = INTEGER;
    $$->value.i = y;
  }|
  C_REAL 
  {
     $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
     char * x= strdup(yytext);
     double y= strtod(x,NULL);
     $$->type = REAL;
     $$->value.r = y;

  }|
  C_CHARACTER
  {
     $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
     char* x= strdup(yytext);
     $$->type = CHARACTER;
     $$->value.c = x[1];
  }|
  C_STRING {
     $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
     char* x= strdup(yytext);
     $$->type = STRING;
     $$->value.s = x;
  }|
  C_TRUE 
  {
    $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
    bool x = true;
    $$->type = BOOLEAN;
    $$->value.b = x;
  }|
  C_FALSE 
  {
    $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
    bool x = false;
    $$->type = BOOLEAN;
    $$->value.b = x;
  }
;
id: 
    ID{
      $$ = (struct Basic*) malloc (sizeof(struct Basic));
      char* x= strdup(yytext);
      $$->s = x;
    }
;
typeName: 
    T_INTEGER{
      $$ = (struct Basic*) malloc (sizeof(struct Basic));
      char* x= "integer";
      $$->s = x;
    }
    |T_REAL{
      $$ = (struct Basic*) malloc (sizeof(struct Basic));
      char* x= "real";
      $$->s = x;
    }|
    T_BOOLEAN{
      $$ = (struct Basic*) malloc (sizeof(struct Basic));
      char* x= "Boolean";
      $$->s = x;
    }|
    T_CHARACTER{
      $$ = (struct Basic*) malloc (sizeof(struct Basic));
      char* x= "character";
      $$->s = x;
    }|
    T_STRING{
      $$ = (struct Basic*) malloc (sizeof(struct Basic));
      char* x= "string";
      $$->s = x;
    }
;


definition_list
: {}
| definition definition_list {
}
;

definition
:TYPE id COLON
{
  struct SymbolTableEntry * record = newRecordEntry($2->s);
  if(lookUp($2->s)!=NULL){
    char  temp[50];
    char  type[50];
    char  temp2[50];
    strcpy(temp,  "ERROR: Type is already defined for type: ");
    strcpy(type,  $2->s);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
  }
  insertToSymboltable(symbolTablecurrent, record);
  newSymbolTable();
}
dblock 
{
  struct SymbolTable * temp = symbolTablecurrent;
  symbolTablecurrent = symbolTablecurrent -> parent; 
  struct SymbolTableEntry * record = findcurrent($2->s);
  record -> entry.rt -> declaration_list = temp;
}
|TYPE id COLON constant ARROW typeName
{
  struct SymbolTableEntry * arrayentry = newArrayEntry($2->s, $4->value.i,$4->value.i,lookUp($6->s), NULL);
  if(lookUp($2->s)!=NULL){
    char  temp[50];
    char  type[50];
    char  temp2[50];
    strcpy(temp,  "ERROR: Type is already defined for type: ");
    strcpy(type,  $2->s);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
  }
  insertToSymboltable(symbolTablecurrent, arrayentry);
}
|TYPE id COLON 
{
  $<symtab>$ = symbolTablecurrent;
  struct SymbolTableEntry * functiontypeentry = newFunctionTypeEntry($2->s, NULL, NULL  );
  if(lookUp($2->s)!=NULL){
    char  temp[50];
    char  type[50];
    char  temp2[50];
    strcpy(temp,  "ERROR: Type is already defined for type: ");
    strcpy(type,  $2->s);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
  }
  insertToSymboltable(symbolTablecurrent, functiontypeentry);
  newSymbolTable();
} pblock ARROW typeName
{
  struct SymbolTableEntry * temp = lookUp($2->s);
  struct FunctionType * funcType = (struct FunctionType *)malloc(sizeof(struct FunctionType));
 	funcType->domainType = symbolTablecurrent; // use a symbol table!
 	funcType->rangeType = lookUp($7->s);
  temp -> entry.ft = funcType;
}
|FUNCTION id COLON id sblock
{
  struct SymbolTableEntry * type = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
  type->name = $4->s;
  struct SymbolTableEntry * funcEntry = newFunctionDefinitionEntry($2->s, type , 0);

  insertToSymboltable(symbolTablecurrent, funcEntry);
  newSymbolTable();
}
;


sblock
:L_BRACE statement_list R_BRACE
{
  struct SymbolTable *temp = symbolTablecurrent;
  newSymbolTable();
  insertToIRArray(createIRLine(NULL, NULL, 10, 0,"main"));
}
| L_BRACE
{
 struct SymbolTable *temp = symbolTablecurrent;
 newSymbolTable();
 insertToIRArray(createIRLine(NULL, NULL, 10, 0,"main"));
}

dblock
statement_list
R_BRACE
{
}
;

dblock
:L_BRACKET declaration_list R_BRACKET
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$ = $2;
}
;

declaration_list
:declaration SEMI_COLON declaration_list 
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$ = $1;
  $$->type = STRING;
}
|declaration
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$ = $1;
  $$->type = STRING;
}
;

declaration
:typeName COLON 
{
  //$<symtabe>$ = malloc(sizeof(*$<symtabe>$));
  $<symtabe>$ = lookUp($1->s);
} 
identifier_list 
;


identifier_list
:id assignOp constant COMMA identifier_list 
|id assignOp constant
|id COMMA 
{
  $<symtabe>$ = $<symtabe>0;
  struct SymbolTableEntry * e = newVariableDeclarationEntry($1->s,$<symtabe>0->entry.pt->size, 0, $<symtabe>0,NULL, false,false);
  if(lookUpSingle(symbolTablecurrent, $1->s)!=NULL){
    char  temp[50];
    char  type[50];
    char  temp2[50];
    strcpy(temp,  "Error Variable is already defined for type: ");
    strcpy(type,  $1->s);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
  }
  insertToSymboltable(symbolTablecurrent,e);
}
identifier_list
|id
{
  $<symtabe>$ = $<symtabe>0;
  struct SymbolTableEntry * e = newVariableDeclarationEntry($1->s,$<symtabe>0->entry.pt->size, 0, $<symtabe>0,NULL, false,false);
  if(lookUpSingle(symbolTablecurrent, $1->s)!=NULL){
    char  temp[50];
    char  type[50];
    char  temp2[50];
    strcpy(temp,  "Error Variable is already defined for type: ");
    strcpy(type,  $1->s);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
  }
  insertToSymboltable(symbolTablecurrent,e);
}
;
statement_list
 :compound_statement statement_list 
 |compound_statement {}
 |simple_statement SEMI_COLON statement_list 
 {
   struct SymbolTableEntry *type = lookUpSingle(symbolTablecurrent,$1->id);

   if(strcmp (type->entry.vd->type->name, "integer") == 0  && strcmp ($1->typeName, "integer") != 0){
     char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "real") == 0 && strcmp ($1->typeName, "real") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "Boolean") == 0 && strcmp ($1->typeName, "Boolean") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "string") == 0 && strcmp ($1->typeName, "string") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "character") == 0 && strcmp ($1->typeName, "character") != 0){
    char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2,"");
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,"");
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
    insertToIRArray(createIRLine(NULL, type, 9, $1->value.i, ""));

 }
 |simple_statement SEMI_COLON 
 {
   struct SymbolTableEntry *type = lookUpSingle(symbolTablecurrent,$1->id);

   if(strcmp (type->entry.vd->type->name, "integer") == 0  && strcmp ($1->typeName, "integer") != 0){
     char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2,"");
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,"");
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "real") == 0 && strcmp ($1->typeName, "real") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "Boolean") == 0 && strcmp ($1->typeName, "Boolean") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "string") == 0 && strcmp ($1->typeName, "string") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
   else if(strcmp (type->entry.vd->type->name, "character") == 0 && strcmp ($1->typeName, "character") != 0){
      char  temp[100];
    char  type1[100];
    char  temp2[100];
    strcpy(temp,  "Error expected type: ");
    strcpy(type1,  type->entry.vd->type->name);
    strcat(temp, type1);
    strcpy(temp2,  ": Actual Type: ");
    strcat(temp, temp2);
    strcpy(temp2, $1->typeName);
    strcat(temp, temp2);
    strcpy(temp2,  "\nAt line Number: ");
    strcat(temp, type);
    strcat(temp, temp2);
    sprintf(lineNum_String, "%d", lineNumber);
    strcat(temp, lineNum_String);
    strcpy(temp2,  " , Column Number: ");
    strcat(temp, temp2);
    sprintf(colNum_String, "%d", columnNumber);
    yyerror(strcat(temp, colNum_String) );
   }
      insertToIRArray(createIRLine(NULL, type, 11, $1->value.i, ""));

 }
;


statement
:compound_statement 
{
}
|simple_statement
{
  $$ = $1;
}
;

compound_statement
 :WHILE L_PARENTHESIS  expression R_PARENTHESIS  statement sblock
 {

 }
 |IF L_PARENTHESIS  expression R_PARENTHESIS  THEN sblock ELSE sblock 
 {

 }
 ;


simple_statement
:assignable assignOp expression
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$->id = $1->id;
  if($3->type == 4){
    $$->value.s = $3->value.s;
    $$->typeName = "string";
  }
  else if($3->type == 3){
    $$->value.c = $3->value.c;
    $$->typeName = "character";
  }
  else if($3->type == 0){
    $$->value.i = $3->value.i;
    $$->typeName = "integer";
  }
  else if($3->type == 2){
    $$->value.b = $3->value.b;
    $$->typeName = "Boolean";
  }
  else if($3->type == 1){
    $$->value.r = $3->value.r;
    $$->typeName = "real";
  }
}
;

assignable
:id 
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$->id = $1->s;
  //$$->typeName = 
}
|assignable ablock
|assignable recOp id
;

expression
:constant
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$ = $1;
}
|preUnaryOperator expression
|expression postUnaryOperator
|assignable
{
  $$ = (struct ConstantValue*) malloc (sizeof(struct ConstantValue));
  $$->actualType = findBasicType("integer");
  $$ = $1;
}
|expression binaryOperator expression
{
  // 0 = add, 1 = sub, 2 = negative, 3 = mul, 4 = div,
  // 5 = rem, 6 = or, 7 = lessthan, 8 = or

  if ($2->lineNo == 0){ //add
    struct ExpressionTypeInfo *exp = (struct ExpressionTypeInfo*) malloc (sizeof(struct ExpressionTypeInfo));
    exp->expectedType = findBasicType("integer");
    $$ = exp;
    insertToIRArray(createIRLine(lookUpSingle(symbolTablecurrent,$1->id), lookUpSingle(symbolTablecurrent, $3->id), 0, 0, ""));
  }
  if ($2 == 1){ //sub
    struct ExpressionTypeInfo *exp = (struct ExpressionTypeInfo*) malloc (sizeof(struct ExpressionTypeInfo));
    exp->expectedType = findBasicType("integer");
    $$ = exp;
    insertToIRArray(createIRLine(lookUpSingle(symbolTablecurrent,$1->id), lookUpSingle(symbolTablecurrent, $3->id), 1, 0, ""));
  }

  if ($2 == 2){ //negative, should never hit
  }

  if ($2 == 3){ // mul

  }
  if ($2 == 4){// div

  }
}
|L_PARENTHESIS expression R_PARENTHESIS
|memOp assignable
;

pblock
:L_PARENTHESIS parameter_list R_PARENTHESIS
{
  $$ = $<symtabe>2;
}
;

parameter_list
:non_empty_parameter_list{

}
|
{
}
;

non_empty_parameter_list
:parameter_declaration COMMA non_empty_parameter_list | parameter_declaration {
  $$ = $1;
};

parameter_declaration
:typeName COLON id
{
  struct SymbolTableEntry *type = findBasicType($1->s);
  $<symtabe>$ = malloc(sizeof(*$<symtabe>$));
  $<symtabe>$ = type;
  struct SymbolTableEntry *state = newVariableDeclarationEntry($3->s, type->entry.pt->size, 0, type, NULL, true, false);
  insertToSymboltable(symbolTablecurrent,state);
}
;

ablock
:L_PARENTHESIS argument_list R_PARENTHESIS
;

argument_list
:non_empty_argument_list | {}
;

non_empty_argument_list
:expression COMMA non_empty_argument_list | expression
;

preUnaryOperator
:SUB_OR_NEG
|ADD
|NOT
|REAL2INT
|INT2REAL
;

postUnaryOperator
:IS_NULL
;

memOp
:RESERVE
|RELEASE
;

assignOp
:ASSIGN 
;

recOp
:DOT
;

binaryOperator
:ADD 
{
  struct Basic *op = (struct Basic *)malloc(sizeof(struct Basic));
  op->lineNo = 0;
  $$ = op;
}
|SUB_OR_NEG
{
  // assuming subtract, need control to figure out which
  struct Basic *op = (struct Basic *)malloc(sizeof(struct Basic));
  op->lineNo = 1;
  $$ = op;
}
|MUL
{
  struct Basic *op = (struct Basic *)malloc(sizeof(struct Basic));
  op->lineNo = 2;
  $$ = op;
}
|DIV
{
  struct Basic *op = (struct Basic *)malloc(sizeof(struct Basic));
  op->lineNo = 3;
  $$ = op;
}
|REM 
|OR
|LESS_THAN
|EQUAL_TO
;
%%
