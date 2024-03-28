#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
	int max_stack_size;
}Stack;

Stack* CreateStack(int max);
void Push(Stack* S, int X);
int Pop(Stack* S);
int Top(Stack* S);

void DeleteStack(Stack* S);
bool IsEmpty(Stack* S);
bool IsFull(Stack* S);

void main(int argc, char *argv[]){
	fin=fopen(argv[1], "r");
	fout=fopen(argv[2], "w");

	Stack* stack;
	char input_str[101];
	int max=20, i=0,a,b;

	fgets(input_str,101,fin);
	stack = CreateStack(max);
	
	fprintf(fout, "top numbers : ");
	while(input_str[i] != '#'){
		if('1'<=input_str[i] && input_str[i]<='9'){
            
            if(IsFull(stack)){
            fprintf(fout, "\nerror : invalid postfix expression, stack is full");
            return;
    }
			Push(stack,input_str[i]-'0');
		}
		else{
            if(IsEmpty(stack)){
                fprintf(fout, "\nerror : invalid postfix expression, stack is empty");
                return;
            }
			int b = Pop(stack);
            if(IsEmpty(stack)){
                fprintf(fout, "\nerror : invalid postfix expression, stack is empty");
                return;
            }
			int a = Pop(stack);
			switch (input_str[i]){
				case '+' : Push(stack, a+b); break;
				case '-' : Push(stack, a-b); break;
				case '*' : Push(stack, a*b); break;
				case '/' : 
                    if(b == 0){
                        fprintf(fout, "\nerror : invalid postfix expression, divide by zero");
                        return;
                    }
                    Push(stack, a/b); break;
				case '%' : 
                    if(b == 0){
                        fprintf(fout, "\nerror : invalid postfix expression, divide by zero");
                        return;
                    }
                    Push(stack, a%b); break;
				default : break;
			}
		}
		fprintf(fout, "%d ", Top(stack));
		i++;
	}
    if(stack->top > 0){
        fprintf(fout, "\nerror : invalid postfix expression, %d elements are left", stack->top + 1);
        return;
    }
	fprintf(fout, "\nevaluation result : %d", stack->key[stack->top]);
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
    Stack* S = (Stack*)malloc(sizeof(malloc));
    S->key = (int*)malloc(sizeof(int) * max);
    S->max_stack_size = max;
    S->top = -1;
    return S;
}

void DeleteStack(Stack* S){
    free(S->key);
    free(S);
	return;
}

void Push(Stack* S, int X){
    
    S->top++;
    S->key[S->top] = X;
	return;
}

int Pop(Stack* S){
    int top_key = S->key[S->top];
    S->top--;
	return top_key;
}

int Top(Stack* S){
    return S->key[S->top];
}

bool IsFull(Stack* S){
    if(S->top == S->max_stack_size - 1){
        return 1;
    }
    return 0;
}

bool IsEmpty(Stack* S){
    if(S->top == -1){
        return 1;
    }
    return 0;
}
