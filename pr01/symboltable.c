#include "symboltable.h"
#include <stdbool.h>
#include "irGeneration.h"

struct SymbolTable * symbolTableglobal;
struct SymbolTable * symbolTablecurrent;
int scope = 0;

struct SymbolTable * newSymbolTable(){
	struct SymbolTable * symbolTable = (struct SymbolTable *)malloc(sizeof(struct SymbolTable));
	symbolTable -> scope = scope;
	int i=0;
	while (i!=99){
		symbolTable ->entries[i]=NULL;
		symbolTable ->child[i]=NULL;
		i++;
	}
	int j = 0;
	if(symbolTablecurrent != NULL){
	while (symbolTablecurrent -> child [j] !=NULL ){
			j++;
	}
		symbolTablecurrent ->child [j] = symbolTable;
	}


	scope++;
	symbolTable -> parent = symbolTablecurrent;
	symbolTablecurrent = symbolTable;

	return symbolTable;
}

struct SymbolTable *initSymboltable(struct SymbolTable *symbolTable){
	//init all built in types to global symbol table
	// ie Integer, Boolean, String, Real, Character, String

	//init int
	struct SymbolTableEntry *IntegerEntry = newPrimitiveEntry("integer", 4);

	//init Boolean
	struct SymbolTableEntry *BooleanEntry = newPrimitiveEntry("Boolean", 1);

	//inti Real
	struct SymbolTableEntry *RealEntry = newPrimitiveEntry("real", 8);

	//init Character
	struct SymbolTableEntry *CharEntry = newPrimitiveEntry("character", 1);

	//init String
	struct SymbolTableEntry *StringEntry = newPrimitiveEntry("string", 100);
	
	//add to golbal table
	symbolTable->entries[0] = IntegerEntry;
	symbolTable->entries[1] = BooleanEntry;
	symbolTable->entries[2] = CharEntry;  
	symbolTable->entries[3] = RealEntry;
	symbolTable->entries[4] = StringEntry;
	
	//return updated table
	return symbolTable;
}

struct SymbolTable * getGlobalTable(){
	return symbolTableglobal;
}

struct SymbolTable * getCurrentTable(){
	return symbolTablecurrent;
}

struct SymbolTableEntry * newFunctionTypeEntry(char * id, struct SymbolTable * domainType, struct SymbolTableEntry * rangeType ){
	struct SymbolTableEntry * funcTypeEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct FunctionType * funcType = (struct FunctionType *)malloc(sizeof(struct FunctionType));
 	funcType->domainType = domainType; // use a symbol table!
 	funcType->rangeType = rangeType;

	funcTypeEntry -> name = id;
	funcTypeEntry -> entry.ft = funcType;
	funcTypeEntry ->variant = FUNCTION_TYPE_DECLARATION;

	return funcTypeEntry;
};

struct SymbolTableEntry * newPrimitiveEntry(char * id, int size){
	struct SymbolTableEntry * primitiveEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct PrimitiveType * primitiveType = (struct PrimitiveType *)malloc(sizeof(struct PrimitiveType));
	
	primitiveType -> size = size;
	primitiveEntry -> name = id;
	primitiveEntry->entry.pt = primitiveType;
	primitiveEntry ->variant = PRIMITIVE_TYPE_DECLARATION;

	return primitiveEntry;
};

struct SymbolTableEntry * newVariableDeclarationEntry(char * id, int size, int offset, struct SymbolTableEntry * type, struct ConstantValue * initialization, bool param, bool hasInit){
	struct SymbolTableEntry * varEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct VariableDeclaration * variableDeclaration = (struct VariableDeclaration *)malloc(sizeof(struct VariableDeclaration));
	
	varEntry -> name = id;
	varEntry -> variant = VARIABLE_DECLARATION;

	variableDeclaration->type = type;
	variableDeclaration->offset = offset;
	variableDeclaration->hasInitialization = hasInit;
	variableDeclaration->parameter = param;
	variableDeclaration->size = size;
	if(hasInit){
		variableDeclaration->initializationValue = initialization;
	}

	varEntry->entry.vd = variableDeclaration;
	return varEntry;
}

struct SymbolTableEntry * newArrayEntry(char * id, int size, int dimensions, struct SymbolTableEntry * type, struct ConstantValue * initialization){
	struct SymbolTableEntry * arrayEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct ArrayType * arrayType = (struct ArrayType *)malloc(sizeof(struct ArrayType));
	struct VariableDeclaration * variableDeclaration = (struct VariableDeclaration *)malloc(sizeof(struct VariableDeclaration));
	// See if entry has Initialization
	if (initialization != NULL){
		variableDeclaration -> hasInitialization = true;
		variableDeclaration -> initializationValue = initialization;
	}
	else{
		variableDeclaration -> hasInitialization = false;
		variableDeclaration -> initializationValue = NULL;

	}
	variableDeclaration -> size = sizeof(* arrayType); // size == |primitive type|, |pointer| for array, record, function
 	variableDeclaration -> offset; // used to determine layout of records, and stack frames
 	variableDeclaration -> type = type;
 	variableDeclaration -> parameter = false; // true --> parameter, false --> local (non-parameter)

	arrayType -> size = size;
	arrayType -> dimensions = dimensions;
	arrayType->declaration = *variableDeclaration;
	arrayEntry -> name = id;
	arrayEntry -> entry.at = arrayType;
	arrayEntry -> variant = ARRAY_TYPE_DECLARATION;

	return arrayEntry;
};

struct SymbolTableEntry * newRecordEntry(char * id){
	struct SymbolTableEntry * recordEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct RecordType * recordType = (struct RecordType *)malloc(sizeof(struct RecordType));
	// Interpreted as the size of the record which is the entry.
	recordType -> size = 1;
	recordEntry -> name = id;
	recordEntry ->entry.rt = recordType;
	recordEntry -> variant = RECORD_TYPE_DECLARATION;

	return recordEntry;
};

struct SymbolTableEntry * newFunctionDefinitionEntry(char * id,struct SymbolTableEntry * type, int codePointer){
	struct SymbolTableEntry * functionDefEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct FunctionDefinition * functionDefinition = (struct FunctionDefinition *)malloc(sizeof(struct FunctionDefinition));
	functionDefinition -> codePointer = codePointer;
	functionDefinition -> type = type;
	functionDefEntry -> name = id;
	functionDefEntry -> entry.fd = functionDefinition;
	functionDefEntry -> variant = FUNCTION_DEFINITION;

	return functionDefEntry;
};

struct SymbolTableEntry * newLocalEntry(char * id, struct SymbolTableEntry * type, struct ConstantValue * initialization){
	struct SymbolTableEntry * localEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct FunctionDefinition * Local = (struct FunctionDefinition *)malloc(sizeof(struct FunctionDefinition));
	localEntry -> name;
	localEntry ->variant;
	localEntry ->entry;

	return localEntry;
};

struct SymbolTableEntry * newParameterEntry(char * id, struct SymbolTableEntry * type){
	struct SymbolTableEntry * param = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct FunctionDefinition * funcdef = (struct FunctionDefinition *)malloc(sizeof(struct FunctionDefinition));
	funcdef->type = type;
	param->name = id;
	param->variant = FUNCTION_DEFINITION;
	param->entry.fd = funcdef;

	return param;

};

struct SymbolTableEntry * newConstantValueEntry(char * id, struct SymbolTableEntry * type, int colNo, int lineNo){
	struct SymbolTableEntry * constantValueEntry = (struct SymbolTableEntry *)malloc(sizeof(struct SymbolTableEntry));
	struct ConstantValue * constantValue = (struct ConstantValue *)malloc(sizeof(struct ConstantValue));
	//constantValue -> actualType -> variant = type->variant;
	constantValue -> lineNo = lineNo;
	constantValue -> colNo = colNo;
	constantValue -> value.b = type->variant;

	constantValueEntry -> name = id; 
	constantValueEntry -> entry;

	return constantValueEntry;
};


struct SymbolTableEntry * findcurrent(char * id){ // looks within current scope
	int i = 0; //iterative variable
	struct SymbolTableEntry *current; //return value

	while (i != 99){ // entries has at most 100 values
		if (symbolTablecurrent->entries[i] == NULL){
			i = 98; // end loop if we hit end of list
		}else{
			if (symbolTablecurrent->entries[i]->name == id){
				current = symbolTablecurrent->entries[i];
				// we hit the correct id
			}
		}
		i++; // increase iterative
	}
	return current; // returns null if not found
}

struct SymbolTableEntry * findall(char * id){
	struct SymbolTableEntry* current; //return value

	int ch = 0; // index of current child
	while (ch != 0){
		int en = 0; // each entry
		ch++; // next child
		while (en != 0){
			if (symbolTableglobal->child[ch]->entries[en]->name == id){
				current = symbolTableglobal->child[ch]->entries[en];
			}
			en++; // next entry
		}
	}
	// now if not found, check the global symbol table
	int i = 0; //iterative variable

	while (i != 99){ // entries has at most 100 values
		if (symbolTableglobal->entries[i] == NULL){
			i = 98; // end loop if we hit end of list
		}else{
			if (symbolTableglobal->entries[i]->name == id){
				current = symbolTableglobal->entries[i];
				// we hit the correct id
			}
		}
		i++; // increase iterative
	}
	return current; // returns null if not found
}

void insertEntry(struct SymbolTableEntry *node){ //inserts to current scope
	int i = 0; // iterative value
	while (i != 99){ // entries will have at most 100 values
		if (symbolTablecurrent->entries[i] == NULL){
			symbolTablecurrent->entries[i] = node;
			i = 98;
			// if at end of current list, insert
		}
		i++;
	// Could fail if the array is full or if malloc does
	// not set unused values to NULL
}
}

void insertToSymboltable(struct SymbolTable * s, struct SymbolTableEntry * e){
	int i = 0;
	char *t = e->name;
		while (s->entries[i] != NULL)
		{
			i++;
		}
		s->entries[i] = e;
	
}

void printExactSymbolTable(struct SymbolTable * temp){

	if (temp != NULL)
	{
		int i = 0;
		while (temp->entries[i] != NULL)
		{
			printf("\n%s", "Name: ");
			printf("%s\n", temp->entries[i]->name);
			printf("%s", "Scope: ");
			printf("%d\n", temp->scope);
			printf("%s", "Type: ");
            if (temp->entries[i]->variant == 5){
                printf("%s\n", temp->entries[i]->entry.vd->type->name);
            }
            else{
                printf("\n");
            }
			if (temp->entries[i]->entry.pt != NULL)
			{
				printf("%s", "Size: ");
				printf("%d\n", temp->entries[i]->entry.pt->size);
			}
			i++;
		}
		int j = 0;
		while (temp->child[j] != NULL)
		{
			printExactSymbolTable(temp->child[j]);
			j++;
		}
	}
}

void printSymbolTable()
{
	int i = 0;
	int j = 0;
	struct SymbolTable *temp = symbolTablecurrent;
	while (temp != NULL)
	{
		while (temp->entries[i] != NULL)
		{
			printf("\n%s", "Name: ");
			printf("%s\n", temp->entries[i]->name);
			printf("%s", "Scope: ");
			printf("%d\n", temp->scope);
			if (temp->entries[i]->entry.pt != NULL)
			{
				printf("%s", "Size: ");
				printf("%d\n", temp->entries[i]->entry.pt->size);
			}
			i++;
		}
		while (temp->child[j] != NULL)
		{
			printf("\n%s", "Name: ");
			printf("%s\n", temp->child[j]->entries[j]->name);
			printf("%s", "Scope: ");
			printf("%d\n", temp->scope);
			if (temp->child[j]->entries[j]->entry.pt != NULL)
			{
				printf("%s", "Size: ");
				printf("%d\n", temp->child[j]->entries[j]->entry.pt->size);
			}
			j++;
		}
		temp = temp->parent;
		i = 0;
		j = 0;
	}
}

struct SymbolTableEntry * findAll(struct SymbolTable * temp, char * n){
	int i=0;
	int j=0;
	if (temp != NULL)
	{
		int i = 0;
		while (temp->entries[i] != NULL)
		{
			if (temp->entries[i]->entry.pt != NULL && strcmp(temp->entries[i]->name, n) == 0)
			{
				return temp->entries[i];
			}
			i++;
		}
		int j = 0;
		while (temp->child[j] != NULL)
		{
			findAll(temp->child[j], n);
			j++;
		}
	}
}

char * lookUpEntryType(char * name,enum DeclarationType * type){
	int i = 0;
	while (symbolTableglobal->entries[i]!=NULL){

		if(strcmp(symbolTableglobal->entries[i]->name , name)==0 && symbolTableglobal->entries[i]->variant == type){
			return symbolTableglobal->entries[i]->name;
		}
		i++;
	}
	return " ";
}

struct SymbolTableEntry *findBasicType(char * type){
	struct SymbolTableEntry *typeEntry; //init return struct

	int i = 0; // iterative value
	while(symbolTableglobal->entries[i]->name != type){ // continue while not at type
		i ++; // add until there
	}
	typeEntry = symbolTableglobal->entries[i]; // set it equal

	return typeEntry; // return
}

struct SymbolTableEntry * lookUp(char * type){
	int i = 0;
	while (symbolTableglobal->entries[i]!=NULL){

		if(strcmp(symbolTableglobal->entries[i]->name , type)==0){
			return symbolTableglobal->entries[i];
		}
		i++;
	}
	return NULL;
}

struct SymbolTableEntry * lookUpSingle(struct SymbolTable* e ,char * type){
	int i = 0;
	while (e->entries[i]!=NULL){
		printf("%s\n", e->entries[i]->name);
		if(strcmp(e->entries[i]->name, type) == 0){
			return e->entries[i];
		}
		i++;
	}
	return NULL;
}