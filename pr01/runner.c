#include <stdio.h>
#include <string.h>
//#include "typedefs.h"
#include <stdlib.h>
#include "grammar.tab.c"
#include "symboltable.h"
#include "irGeneration.c"
#include "target-x86.c"

/*

    Assists in running flex file and creating output.

    Created on Feb 6 2017
    Last modified on Feb 7 2022 by Carl Alphonce

*/

extern int yyparse();
extern int yyset_in();
extern int yylineno;
extern char *yytext;
extern int lineNumber;
extern int columnNumber;
extern FILE *yyin;
extern FILE *yyout;


int main(int argc, char *argv[])
{
    int token = -999;
    columnNumber = 1;
    lineNumber = 1;

    /*
       Code needs to be added to handle command line arguments,
       (keeping in mind only -help and -tok have meaning so far)
       open the input file, open the output file, etc.
    */

    if (strcmp(argv[1], "-tok") == 0)
    {
        yyin = fopen(argv[2], "r");
        char *something = (char *)malloc(40);
        strcpy(something, argv[2]);
        FILE *out_file = fopen(strcat(argv[2], ".tok"), "w");
        token = yylex();
        fprintf(out_file, "%s%s\n", "alpha parser, version 2022.01 - Tokens for file ", something);
        do
        {
            fprintf(out_file, "%3d %s %d %d\n", token, yytext, lineNumber, columnNumber);
            if ((int)token == 700)
            {
                char *x = strchr(yytext, '\n');
                while (x != NULL)
                {
                    lineNumber++;
                    x = strchr(x + 1, '\n');
                }
            }
            columnNumber += strlen(yytext);
            token = yylex();

        } while (token != 0);
    }
    if (strcmp(argv[1], "-st") == 0)
    {
        symbolTableglobal = newSymbolTable();
        symbolTableglobal->parent = NULL;
        symbolTableglobal = initSymboltable(symbolTableglobal);
        //printf("%s",lookUp("Real")->name);
        symbolTablecurrent = symbolTableglobal;
        yyin = fopen(argv[2], "r");
        yyset_in(yyin);
        yyparse();


        printExactSymbolTable(symbolTableglobal);
    }
    if (strcmp(argv[1], "-asc") == 0)
    {

    }
    if (strcmp(argv[1], "-tc") == 0)
    {
        printf("HELP:\nHow to run the compiler:\n./compiler [options] program\nValid Options:\n");
    }
    if (strcmp(argv[1], "-ir") == 0)
    {
        symbolTableglobal = newSymbolTable();
        symbolTableglobal->parent = NULL;
        symbolTableglobal = initSymboltable(symbolTableglobal);
        initIRArray();
        //printf("%s",lookUp("Real")->name);
        symbolTablecurrent = symbolTableglobal;
        yyin = fopen(argv[2], "r");
        yyset_in(yyin);
        yyparse();
        printExactSymbolTable(symbolTableglobal);

        // create and malloc irarray
        //print out the array
        printIRarray();
    }
    if (strcmp(argv[1], "-cg") == 0)
    {
        parse();
        printf("HELP:\nHow to run the compiler:\n./compiler [options] program\nValid Options:\n");
    }
    if (strcmp(argv[1], "-debug") == 0)
    {
        printf("HELP:\nHow to run the compiler:\n./compiler [options] program\nValid Options:\n");
    }
    if (strcmp(argv[1], "-help") == 0)
    {
        printf("HELP:\nHow to run the compiler:\n./compiler [options] program\nValid options:\n-tok output the token number, token, line number, and column number for each of the\ntokens to the .tok file\n-st output the symbol table for the program to the .st file\n-asc output the annotated source code for the program to the .asc file\n-tc run the type checker and report type errors\n-ir run the intermediate representation generator, writing output to the .ir file\n-cg run the (x86 assembly) code generator, writing output to the .s file\n-debug produce debugging messages to stderr\n-help print this message and exit the compiler\n");
    }
    /*
       Don't forget to do appropriate clean-up (like closing files).
    */

    return 0;
}
