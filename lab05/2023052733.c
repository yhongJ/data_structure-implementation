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
	//root부터 차례로 내려가면서 key가 들어갈 장소를 재귀적으로 탐색
	if(current == NULL){
		current = (BSTNode*)malloc(sizeof(BSTNode));
		current->value = key;
		current->left = NULL;
		current->right = NULL;
	}
	else if(key < current->value){
		//key가 value보다 작으면 current의 왼쪽으로 재귀
		current->left = insertNode(current->left, key);
	}
	else if(key > current->value){
		//key가 value보다 크면 current의 오른쪽으로 재귀
		current->right = insertNode(current->right, key);
	}
	else{
		printf("insertion error %d", key);
	}
    return current;
}

BSTNode* deleteNode(BSTNode* current, int key){
	
	if(current == NULL){
		return current;
	}
	if(key < current->value){
		//key가 value보다 작으면 current의 왼쪽으로 재귀
		current->left =  deleteNode(current->left, key);
	}
	else if(key > current->value){
		//key가 value보다 크면 current의 오른쪽으로 재귀
		current->right = deleteNode(current->right, key);
	}
	//삭제해야할 node를 찾았을때
	else{
		//child가 0개인 경우
		if(current->right == NULL && current->left == NULL){
			free(current);
			return NULL;
		}
		//child가 1개인 경우
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
		//child가 2개인 경우 -> 오른쪽 subtree의 가장 왼쪽 노드를 찾아서 replacement->child가 하나인 경우로 바뀜
		else {
			BSTNode* successor = current->right;
			BSTNode* successorParent = current;
			while (successor->left != NULL) {
				successorParent = successor;
				successor = successor->left;
			}
			current->value = successor->value; 
			if (successorParent == current)
				successorParent->right = deleteNode(successor, successor->value);
			else
				successorParent->left = deleteNode(successor, successor->value);
		}
	}
	

    return current;
}

BSTNode* findNode(BSTNode* current, int key){
	if(current == NULL){
		return current;
	}
	//insert node와 같은 방식으로 root부터 시작해서 재귀적으로 탐색
	if(key < current->value){
		current = findNode(current->left, key);
	}
	else if(key > current->value){
		current = findNode(current->right, key);
	}
	else{
		return current;
	}
    
}
void printInorder(BSTNode* current){
	//inorder 순회과정을 재귀적으로 출력
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
	//재귀적으로 left subtree삭제 -> right subtree삭제 -> node삭제
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