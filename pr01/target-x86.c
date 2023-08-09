#include "irGeneration.h"
#include "symboltable.h"
#include "target-x86.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* replaceWord(const char* s, const char* oldW,const char* newW)
{
    char* result;
    int i, cnt = 0;
    int newWlen = strlen(newW);
    int oldWlen = strlen(oldW);
    for (i = 0; s[i] != '\0'; i++) {
        if (strstr(&s[i], oldW) == &s[i]) {
            cnt++;
            i += oldWlen - 1;
        }
    }
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1);
    i = 0;
    while (*s) {
        if (strstr(s, oldW) == s) {
            strcpy(&result[i], newW);
            i += newWlen;
            s += oldWlen;
        }
        else
            result[i++] = *s++;
    }
    result[i] = '\0';
    return result;
}

char move[] = 
    "movl    $, %s(%rbp)";
char start[] = 
    "pushq   %rbpn\n"
    "movq    %rsp, %rbp\n"
    "movl    %edi, %s(%rbp)\n"
    "movl    %esi, %s(%rbp)\n";
char ending[] = 
    "popq    %rbp\n";
char add[] =
    "movl    %s(%rbp), %edx\n"
    "movl    %s(%rbp), %eax\n"
    "addl    edx, %eax\n";
char sub[] = 
    "subl    %s(%rbp), %eax\n";
char times[] =  
    "imull   %s(%rbp), %eax\n";
char divd[] = 
    "movl    %s(%rbp), %eax\n"
    "dcltd\n"
    "idivl   %s(%rbp)\n";

// struct Node *root = NULL;
// int leadersIdx[1000];
// int leadersMaxIdx = 0;
// bool skipOne = false;
    
// operator can be:
// 0 = add, 1 = sub, 2 = negative, 3 = mul, 4 = div,
// 5 = rem, 6 = or, 7 = lessthan, 8 = or

void parse(){
    struct irLine **cursor= irCode;
    int maxIdx = end;
    for (int i; i <= maxIdx;i++ ){
        int ops = cursor[i] -> operator;
        if(ops == 9){
            int offS = cursor[i] ->left ->entry.vd-> offset;
            char result[500];
            sprintf(result, "%d", offS);
            char *str= replaceWord(move, "%s",result);
            printf("%s",str);
            printf("%s",ending);

        }
        if(ops == 0){
            int offSL = cursor[i] ->left ->entry.vd-> offset;
            int offSR = cursor[i] ->right ->entry.vd-> offset;
            // int offStmp = cursor[i] ->tmp ->entry.vd-> offset;
            char result1[500];
            sprintf(result1, "%d", offSL);
            // char *str1= replaceWord(add, "%s",result1);
            // printf("%s",str1);
            // printf("%s",str);

            char result2[500];
            sprintf(result2, "%d", offSR);
            char *str2= replaceWord(add, "%s",result2);
            printf("%s",str2);
            printf("%s",ending);


        }
        else if(ops==1){

        }
        else if(ops==3){

        }
        else if(ops==4){

        }

        // idLeaders(cursor[i]);
    }

    
    // int nextLeadIdx = 0;
    // for (int i; i <= maxIdx;i++ ){
    //     if (i <= leadersIdx[nextLeadIdx]){

    //     }
    //     else{
    //         nextLeadIdx +=1 ;
    //     }
    // }
}



  
// printf("%s%s%s",start,div,end);



// void appendToLeadersIdx(int idx){
//     leadersMaxIdx += 1;
//     leadersIdx[leadersMaxIdx]=idx;
// }

// void idLeaders(struct irLine * line){
//     struct irLine *cursor = line;
//     if (!skipOne){
//         skipOne = false;
//         if (line-> index == 0){
//             appendToLeadersIdx(0);
//         }
//         else if (line->operator){
//             appendToLeadersIdx(line->index+1);

//             // appendToLeadersIdx(line->goTo);
//         }
//     }
// }

// struct Node* createNode(){
//     struct Node *node = (struct Node *) malloc(sizeof(struct Node));
// }

// void appendLinesToNode(struct Node *node, struct irLine *lines){
//     node -> maxLineIdx += 1;
//     node ->lines[node -> maxLineIdx] = lines;
// }

// void appendEdgesToNode(struct Node *node, struct irLine *lines){
//     node -> maxEdgeIdx += 1;
//     node ->lines[node -> maxLineIdx] = lines;
// }

// void insertToDag(){
//     struct node;
// }

// void swap(int* xp, int* yp)
// {
// 	int temp = *xp;
// 	*xp = *yp;
// 	*yp = temp;
// }

// void selectionSort(int arr[], int n)
// {
// 	int i, j, min_idx;
// 	for (i = 0; i < n - 1; i++) {
// 		min_idx = i;
// 		for (j = i + 1; j < n; j++)
// 			if (arr[j] < arr[min_idx])
// 				min_idx = j;
// 		swap(&arr[min_idx], &arr[i]);
// 	}
// }

// int main()
// {
// 	int arr[] = { 0, 23, 14, 12, 9 };
// 	int n = leadersMaxIdx + 1;
// 	selectionSort(arr, n);
// 	return 0;
// }
