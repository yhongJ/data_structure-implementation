#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct BinaryTree TREE;

struct BinaryTree{
    int data;
    TREE* left_pointer;
    TREE* right_pointer;
};

void CreateFromArray(TREE* current_tree, int current_index, int* inserted_data_array, int length);
void DeleteTree(TREE* current);
void printPreorder(TREE* current);
void printInorder(TREE* current);
void printPostorder(TREE* current);


void DeleteTree(TREE* current){
    //재귀적으로 왼쪽 child가 없을 때까지 왼쪽 subtree삭제
    if(current->left_pointer != NULL){
        DeleteTree(current->left_pointer);
    }
    //재귀적으로 오른쪽 child가 없을 때까지 오른쪽 subtree삭제
    if(current->right_pointer != NULL){
        DeleteTree(current->right_pointer);
    }
    if(current != NULL){
        free(current);
    }
    return;
    

}
void printPreorder(TREE* current){
    //preorder 순회값을 출력 (node -> 왼쪽 subtree -> 오른쪽 subtree )
    if(current != NULL){
        fprintf(fout, "%d ", current->data);
    }
    if(current->left_pointer != NULL){
        printPreorder(current->left_pointer);
    }
    if(current->right_pointer != NULL){
        printPreorder(current->right_pointer);
    }
    
    
    return;
}
void printInorder(TREE* current){
    //inorder 순회값을 출력 (왼쪽 subtree -> node -> 오른쪽 subtree)
    if(current->left_pointer != NULL){
        printInorder(current->left_pointer);
    }
    if(current != NULL){
        fprintf(fout, "%d ", current->data);
    }
    if(current->right_pointer != NULL){
        printInorder(current->right_pointer);
    }
    return;
}
void printPostorder(TREE* current){
    //postorder 순회값을 출력 (왼쪽 subtree -> 오른쪽 subtree -> node)
    if(current->left_pointer != NULL){
        printPostorder(current->left_pointer);
    }
    if(current->right_pointer != NULL){
        printPostorder(current->right_pointer);
    }
    if(current != NULL){
        fprintf(fout, "%d ", current->data);
    }
    
    return;
}
void CreateFromArray(TREE* current, int current_index, int* inserted_data_array, int length){
    //index가 1부터 시작하므로 왼쪽child의 index는 2 * current_index, 오른쪽 child index는 2 * current_index + 1
    //재귀적으로 노드 생성
    if(current != NULL){
        current->data  = inserted_data_array[current_index];
    }
    
    //왼쪽 child생성
    if(2 * current_index <= length - 1){
        current->left_pointer = (TREE*)malloc(sizeof(TREE));
        CreateFromArray(current->left_pointer, 2 * current_index , inserted_data_array, length);
    }

    else{
        current->left_pointer = NULL;
    }
    
    //오른쪽 child생성
    if(2 * current_index + 1 <= length - 1){
        current->right_pointer =  (TREE*)malloc(sizeof(TREE));
        CreateFromArray(current->right_pointer, 2 * current_index + 1 , inserted_data_array, length);
    }

    else{
        current->right_pointer = NULL;
    }

    return;



}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int input_data;
	int NUM_NODES;
	fscanf(fin, "%d", &NUM_NODES);
	int* array = (int*)malloc(sizeof(int) * (NUM_NODES + 1));
	array[0] = -1;
	for(int i = 0; i < NUM_NODES; i++){
	    fscanf(fin, "%d", &input_data);
	    array[i + 1] = input_data;
	}
	TREE* header = (TREE*)malloc(sizeof(TREE));
	CreateFromArray(header, 1, array, NUM_NODES + 1);
	printPreorder(header);
	fprintf(fout, "\n");
	printInorder(header);
	fprintf(fout, "\n");
	printPostorder(header);
	DeleteTree(header);
	fclose(fin);
	fclose(fout);
	return 0;
}