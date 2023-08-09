#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef IRGENERATION_H
#define IRGENERATION_H

// operator can be:
// 0 = add, 1 = sub, 2 = negative, 3 = mul, 4 = div,
// 5 = rem, 6 = or, 7 = lessthan, 8 = or, 9 = assignment, 10 = labels
//

struct irLine {
    int index;
    struct SymbolTableEntry * right;
    struct SymbolTableEntry * left;
    int operator;
    int rightINT;
    char * label;
};

// function declarations
void * initIRArray();
void insertToIRArray(struct irLine *line);
struct irLine * createIRLine(struct SymbolTableEntry * right, struct SymbolTableEntry * left, int operator, int value, char * label);
void populateIRarray();
void printIRarray();
//asd


#endif

