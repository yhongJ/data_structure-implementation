#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct Stack{
	int *key;
	int top;
    //top: stack의 맨 윗 값의 index
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
    //모든 예외처리는 프로그램자체를 종료해야하기 때문에 main code에 구현
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
            //예외처리)stack이 차있는 상태에서 push
            if(IsFull(stack)){
            fprintf(fout, "\nerror : invalid postfix expression, stack is full");
            return;
    }
			Push(stack,input_str[i]-'0');
		}
		else{
            //예외처리) stack에 두개 이상의 숫자가 없는 상태에서 연산자가 들아왔을 때
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
                        //예외처리) 0으로 나눔
                        fprintf(fout, "\nerror : invalid postfix expression, divide by zero");
                        return;
                    }
                    Push(stack, a/b); break;
				case '%' : 
                    if(b == 0){
                        //예외처리) 0으로 나눔
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
        //예외처리) 연산결과가 하나로 나오지않았는데 expression이 종료
        fprintf(fout, "\nerror : invalid postfix expression, %d elements are left", stack->top + 1);
        return;
    }
	fprintf(fout, "\nevaluation result : %d", stack->key[stack->top]);
	fclose(fin);
	fclose(fout);
	DeleteStack(stack);
}

Stack* CreateStack(int max){
    //stack을 생성하고 top index 에 -1 할당
    Stack* S = (Stack*)malloc(sizeof(malloc));
    S->key = (int*)malloc(sizeof(int) * max);
    S->max_stack_size = max;
    S->top = -1;
    return S;
}

void DeleteStack(Stack* S){
    //key부터 free한 후 stack을 free
    free(S->key);
    free(S);
	return;
}

void Push(Stack* S, int X){
    //top의 인덱스를 1증가시키고 key[top]에 X할당
    S->top++;
    S->key[S->top] = X;
	return;
}

int Pop(Stack* S){
    //key[top]을 return, top의 인덱스 1감소
    int top_key = S->key[S->top];
    S->top--;
	return top_key;
}

int Top(Stack* S){
    //key[top] return
    return S->key[S->top];
}

bool IsFull(Stack* S){
    //top의 index와 stack의 크기가 같으면 full
    if(S->top == S->max_stack_size - 1){
        return 1;
    }
    return 0;
}

bool IsEmpty(Stack* S){
    //top의 index가 -1이면 empty
    if(S->top == -1){
        return 1;
    }
    return 0;
}
