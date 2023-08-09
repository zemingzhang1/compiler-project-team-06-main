#include "irGeneration.h"
#include "symboltable.h"

// Global ir array
// each entry is a line of IR code
struct irLine * irCode[1000];
int end;

void *initIRArray() {

    // iterative value
    int i = 0;
    end = 0;

    // malloc irCode to allow 1000 lines
    //irCode = malloc(1000 * sizeof(int));

    // clear all memory in arrays path
    while (i != 1000){
        irCode[i] = NULL;
        i++;
    }
}

void insertToIRArray(struct irLine * line){
  irCode[end] = line;
  line->index = end;
  end++;
}

struct irLine * createIRLine(struct SymbolTableEntry * right, struct SymbolTableEntry * left, int operator, int value, char * label){
  struct irLine * newLine = (struct irline *) malloc(sizeof(struct irLine));
  if (operator == 10){
    newLine->label = "main";
    newLine->operator = 10;
  }else{
    if (operator == 9){
      newLine->rightINT = value;
      newLine->left = left;
      newLine->operator = operator;
    }else{
      if (operator == 11){
        newLine->left = left;
        newLine->operator = operator;
      }else{
        newLine->left = left;
        newLine->right = right;
        newLine->operator = operator;
      }
    }
  }

  return newLine;
}

void populateIRarray(){
	int i = 0;
	int j = 0;
	struct SymbolTable * temp = symbolTableglobal;

  while (temp != NULL){
	while(temp->entries[i] != NULL){

		if (temp->entries[i]->entry.pt != NULL){

		}
		i++;
	}
	while(temp->child[j] != NULL){

		if (temp->child[j]->entries[j]->entry.pt != NULL){

		}
		j++;
	}
	temp = temp->parent;
	i=0;
	j=0;
	}
}

void printIRarray(){
  // iterator
  int i = end - 1;
  FILE *out_file = fopen("ir.ir", "w");
  fprintf(out_file,"IR Code: \n");
  fprintf(out_file,"%s\n", irCode[0]->label);
  while (i != 0){
    if (irCode[i]->operator == 9){
      fprintf(out_file, "%s", irCode[i]->left->name);
      fprintf(out_file, "=");
      fprintf(out_file, "%d\n", irCode[i]->rightINT);
    }
    if (irCode[i]->operator == 11){
      fprintf(out_file, "%s", irCode[i]->left->name);
      fprintf(out_file, "=");
    }
    if (irCode[i]->operator == 0){
      fprintf(out_file, "%s", irCode[i]->left->name);
      fprintf(out_file, "+");
      fprintf(out_file, "%s\n", irCode[i]->right->name);
    }
    if (irCode[i]->operator == 1){
      fprintf(out_file, "%s", irCode[i]->left->name);
      fprintf(out_file, "-");
      fprintf(out_file, "%s\n", irCode[i]->right->name);
    }
    i--;
  }
}