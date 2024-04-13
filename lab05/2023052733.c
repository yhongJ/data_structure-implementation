#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;


typedef struct BinarySearchTreeNode BSTNode;
struct BinarySearchTreeNode{
	int value;
	BSTNode* left;
	BSTNode* right;
};

BSTNode* insertNode(BSTNode* current, int key);
BSTNode* deleteNode(BSTNode* current, int key);
BSTNode* findNode(BSTNode* current, int key);
void printInorder(BSTNode* current);
void deleteTree(BSTNode* current);

BSTNode* insertNode(BSTNode* current, int key){
	if(current == NULL){
		current = (BSTNode*)malloc(sizeof(BSTNode));
		current->value = key;
		current->left = NULL;
		current->right = NULL;
	}
	else if(key < current->value){
		current = insertNode(current->left, key);
	}
	else if(key > current->value){
		current = insertNode(current->right, key);
	}
	else{
		printf("insertion error %d", key);
	}
    //if current == NULL일 경우 할당
    //elif key < current->value 일 경우 recursive
    //elif key > current->value 일 경우 recursive
    //else 에러
    //return current
    return current;
}
//오른쪽 자식 기준
BSTNode* deleteNode(BSTNode* current, int key){
	if(current == NULL){
		return current;
	}
	if(key < current->value){
		current =  deleteNode(current->left, key);
	}
	else if(key > current->value){
		current = deleteNode(current->right, key);
	}
	else{
		if(current->right == NULL && current->left == NULL){
			free(current);
			return NULL;
		}

		else if(current->right == NULL){
			BSTNode* tmp = current->left;
			free(current);
			return tmp;
		}
		else if(current->left == NULL){
			BSTNode* tmp = current->right;
			free(current);
			return tmp;
		}
		else{
			BSTNode* successor = current->right;
			while(successor->left != NULL){
				successor = successor->left;
			}
			int tmp = current->value;
			current->value = successor->value;
			successor->value = tmp;
			return deleteNode(current-> right, key);
			
		}
	}
	
    //current == NULL일경우 return current
    //if   key < current->value 일 경우 recursive
    //elif key > current->value 일 경우 recursive
    //else 의 경우 4가지
    //  자식이 둘일 경우 -> 우측 자식의 가장 좌측 자식 찾기 -> 그 노드의 값으로 current->value 설정 -> 우측 자식의 가장 좌측 자식 삭제 recursive
    //  자식이 하나일 경우 -> free current -> return 자식
    //  자식이 없을 경우 -> free current -> return NULL
    //return current
    return current;
}

BSTNode* findNode(BSTNode* current, int key){
	if(current == NULL){
		return current;
	}
	if(key < current->value){
		current = findNode(current->left, key);
	}
	else if(key > current->value){
		current = findNode(current->right, key);
	}
	else{
		return current;
	}
    //current == NULL일경우 return current
    //if   key < current->value 일 경우 current->left에 대해 recursive
    //elif key > current->value 일 경우 current->right에 대해 recursive
    //else return current
}
void printInorder(BSTNode* current){
	if(current->left != NULL){
		printInorder(current->left);
	}
	fprintf(fout, "%d ", current->value);
	if(current->right != NULL){
		printInorder(current->right);
	}
	return;
}

void deleteTree(BSTNode* current){
	if(current->left != NULL){
		deleteTree(current->left);
	}
	if(current->right != NULL){
		deleteTree(current->right);
	}
	free(current);
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char cv;
	int key;

	BSTNode* root = NULL;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'i':
				fscanf(fin, "%d", &key);
				root = insertNode(root, key);
				break;
			case 'f':
				fscanf(fin,"%d",&key);
				if(findNode(root, key)){
					fprintf(fout, "%d is in the tree\n", key);
				}else{
					fprintf(fout, "%d is not in the tree\n", key);
				}
				break;
			case 'd':
				fscanf(fin, "%d", &key);
				if(findNode(root, key)){
					root = deleteNode(root, key);
				}else{
					fprintf(fout, "deletion error %d\n", key);
				}
				break;
			case 'p':
				fscanf(fin, "%c", &cv);
				if(cv == 'i'){
					if(root == NULL){
						fprintf(fout, "tree is empty");
					}else{
						printInorder(root);
					}
				}
				fprintf(fout, "\n");
				break;
		}
	}
	deleteTree(root);
}