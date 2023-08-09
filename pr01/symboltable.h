#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef SYMBOLTABLE_H
#define SYMBOLTABLE_H
enum DeclarationType { PRIMITIVE_TYPE_DECLARATION,
ARRAY_TYPE_DECLARATION, RECORD_TYPE_DECLARATION,
FUNCTION_TYPE_DECLARATION, FUNCTION_DEFINITION,
VARIABLE_DECLARATION};

enum ConstantType {INTEGER, REAL, BOOLEAN, CHARACTER, STRING };

struct SymbolTableEntry {
 char * name;
 enum DeclarationType *variant;
 union {
 struct PrimitiveType *pt;
 struct ArrayType *at;
 struct RecordType *rt;
 struct FunctionType *ft;
 struct FunctionDefinition *fd;
 struct VariableDeclaration *vd;
 } entry;
};

struct ConstantValue {
 struct SymbolTableEntry * actualType;
 int lineNo;
 int colNo;
 enum ConstantType type;
 union {
 int i;
 double r;
 bool b;
 char c;
 char * s;
 } value;
char * id;
char * typeName;
};

struct Basic {
 int lineNo;
 int colNo;
 char * s;
};

struct ExpressionTypeInfo {
 struct SymbolTableEntry * actualType;
 struct SymbolTableEntry * expectedType;
 struct InstructionList * trueList;
 struct InstructionList * falseList;
 struct SymbolTableEntry * addr;
};

struct StatementInfo {
 int next; /* instruction following this statement */
};

struct SymbolTable{
	struct SymbolTable * parent;
	struct SymbolTable *child[100];
	int scope;
	struct SymbolTableEntry *entries[100];
};

struct PrimitiveType {
 int size; // in bytes
};

struct RecordType {
 int size;
 struct SymbolTable * declaration_list; // use a symbol table!
};

struct FunctionType {
 int size;
 struct SymbolTable * domainType; // use a symbol table!
 struct SymbolTableEntry * rangeType;
};

struct FunctionDefinition {
 struct SymbolTableEntry * type; // this will be a FunctionType
 int codePointer; // type may not be correct
};

struct VariableDeclaration {
 int size; // size == |primitive type|, |pointer| for array, record, function
 int offset; // used to determine layout of records, and stack frames
 struct SymbolTableEntry * type;
 bool parameter; // true --> parameter, false --> local (non-parameter)
 bool hasInitialization;
 struct ConstantValue * initializationValue;
};

struct ArrayType {
 int size;
 int dimensions;
 struct VariableDeclaration declaration;
};

struct ParameterDec {
	struct Basic *right;
	struct Basic *left;
};

struct Local {
	int size;
};

struct SymbolTableEntry * newFunctionTypeEntry(char * id, struct SymbolTable * domainType, struct SymbolTableEntry * rangeType);
struct SymbolTable * newSymbolTable();
struct SymbolTableEntry * newPrimitiveEntry(char * id, int size);
struct SymbolTableEntry * newArrayEntry(char * id, int size, int dimensions, struct SymbolTableEntry * type, struct ConstantValue * initialization);
struct SymbolTableEntry * newRecordEntry(char * id);
struct SymbolTableEntry * newFunctionDefinitionEntry(char * id,struct SymbolTableEntry * type, int codePointer);
struct SymbolTableEntry * newLocalEntry(char * id, struct SymbolTableEntry * type, struct ConstantValue * initialization);
struct SymbolTableEntry * newParameterEntry(char * id, struct SymbolTableEntry * type);
struct SymbolTableEntry * ConstantValue(char * id, struct SymbolTableEntry * type, int colNo, int lineNo);
struct SymbolTableEntry * findcurrent(char * id);
struct SymbolTableEntry * findall(char * id);
void insertToSymboltable(struct SymbolTable * s, struct SymbolTableEntry * e);
struct SymbolTable * getCurrentTable();
struct SymbolTable * getGlobalTable();
struct SymbolTableEntry *findBasicType(char * type);
struct SymbolTableEntry * newVariableDeclarationEntry(char * id, int size, int offset, struct SymbolTableEntry * type, struct ConstantValue * initialization, bool param, bool hasInit);
void printSymbolTable();
struct SymbolTableEntry * findAll(struct SymbolTable * temp, char * n);
struct SymbolTableEntry * lookUpSingle(struct SymbolTable* e ,char * type);
char * lookUpEntryType(char * name,enum DeclarationType * type);
struct SymbolTableEntry * lookUp(char * type);
// void insertEntry(struct SymbolTableEntry *node);



#endif

