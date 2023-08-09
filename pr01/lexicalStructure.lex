%{

/*
    C code can go in this section

    #include "typedefs.h" shown to encapsulate those definitions in separate file
*/

#include <string.h>
#include "grammar.tab.h"

int lineNumber =0;
int columnNumber =0;

%}
    /* Define FLEX abbreviations - sample provided */
DIGIT		[0-9]*
ID          [_a-zA-Z][_a-zA-Z0-9]*
COMMENT     "(*"[^*)]*(.|\n)"*)" 
STRING      "\""(.+?)\"""
FLOAT       ([0-9.(0-9)]+)|([0-9]*\.?[0-9]['E'|'e'][-|+]?[0-9]*\.?)
CHARACTER   '[a-z0-9A-Z]'|("'\\n'")|("'\\t'")|("'\\''")|("'\\\\'")|(['\\'('"')]+)

%%
    /* Give FLEX rules: "real" is a regex ... and when recognized the lexer will return the constant T_REAL,  */
    /* defined in typedefs.h                                                                                  */
["/n"]                          ++lineNumber; columnNumber=1;
[\n]                            ++lineNumber; columnNumber=1;
[/n]                            ++lineNumber; columnNumber=1;
[" "]                           ++columnNumber;

    /* Types */
"string"                        return T_STRING;
"Boolean"                       return T_BOOLEAN;
"integer"			      	    return T_INTEGER;
"real"			      	        return T_REAL;
"character"                     return T_CHARACTER;

    /* Keywords */
"null"                          return NULL_PTR;
"for"                           return FOR;
"while"                         return WHILE;
"if"                            return IF;
"then"                          return THEN;
"else"                          return ELSE;
"switch"                        return SWITCH;
"case"                          return CASE;
"otherwise"                     return OTHERWISE;
"type"                          return TYPE;
"function"                      return FUNCTION;
    /* punctuation - grouping */
"("                             return L_PARENTHESIS;
")"                             return R_PARENTHESIS;
"["                             return L_BRACKET;
"]"                             return R_BRACKET;
"{"                             return L_BRACE;
"}"                             return R_BRACE;
    /* punctuation - other */
";"                             return SEMI_COLON;
":"                             return COLON;
","                             return COMMA;
"->"                            return ARROW;
"."                             return DOT;
    /* operators */ 
"+"                             return ADD;
"-"                             return SUB_OR_NEG;
"*"                             return MUL;
"/"                             return DIV;
"%"                             return REM;
"<"                             return LESS_THAN;
"="                             return EQUAL_TO;
":="                            return ASSIGN;
"i2r"                           return INT2REAL;
"r2i"                           return REAL2INT;
"isNull"                        return IS_NULL;
"!"                             return NOT;
"&"                             return AND;
"|"                             return OR;
"reserve"                       return RESERVE;
"release"                       return RELEASE;

"true"                          return C_TRUE;      
"false"                         return C_FALSE;

{DIGIT}                         return C_INTEGER;
{ID}                            return ID;
{COMMENT}                       return COMMENT;
{STRING}                        return C_STRING;
{FLOAT}                         return C_REAL;
{CHARACTER}                     return C_CHARACTER;
%%


int yywrap(void){
    return 1;
}