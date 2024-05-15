#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct AVLTreeNode AVLNode;
typedef int Element;

struct AVLTreeNode {
    Element element;
    AVLNode* left;
    AVLNode* right;
    int height;
};

int max(int a, int b) {
    return a > b ? a : b;
}

int height(AVLNode* node) {
    //만약 node가 null 이라면 0을 return
    if(node == NULL) return 0;
    return node->height;
}

AVLNode* RotateRight(AVLNode* node) {
    //기존 노드(exNode), 노드의 왼쪽 포인터(exLeft) 를 선언
    AVLNode* exLeft = node->left;
    AVLNode* exNode = node;
    //노드는 exLeft로, 노드의 오른쪽포인터는 exNode로 update
    node = exLeft;
    exNode->left = node->right;
    node->right = exNode;
    //깊이는 자식들의 max height + 1
    exNode->height = max(height(exNode->left), height(exNode->right)) + 1;
    node->height = max(height(node->left), height(node->right)) + 1;

    return node;
}

AVLNode* RotateLeft(AVLNode* node) {
    //기존 노드(exNode), 노드의 오른쪽 포인터(exRight) 를 선언
    AVLNode* exRight = node->right;
    AVLNode* exNode = node;
    //노드는 exRight로, 노드의 왼쪽 포인터는 exNode로 update
    node = exRight;
    exNode->right = node->left;
    node->left = exNode;
    //깊이는 자식들의 max height + 1
    exNode->height = max(height(exNode->left), height(exNode->right)) + 1;
    node->height = max(height(node->left), height(node->right)) + 1;

    return node;
}

AVLNode* rebalance(AVLNode* T) {
    //L이 deepest한 경우
    if (height(T->left) > height(T->right) + 1) {
        //L->R이 deepest한 경우
        if (height(T->left->right) > height(T->left->left)) {
            T->left = RotateLeft(T->left);
        }
        T = RotateRight(T);
    //R이 deepest한 경우    
    } else if (height(T->left) + 1 < height(T->right)) {
        //R->L이 deepest한 경우
        if (height(T->right->right) < height(T->right->left)) {
            T->right = RotateRight(T->right);
        }
        T = RotateLeft(T);
    }
    T->height = max(height(T->left), height(T->right)) + 1;
    return T;
}

AVLNode* Insert(Element X, AVLNode* T) {
    //T가 null인경우 새로운 AVLnode를 만들고 초기값 할당(height == 1)
    if (T == NULL) {
        T = (AVLNode*)malloc(sizeof(AVLNode));
        T->element = X;
        T->left = NULL;
        T->right = NULL;
        T->height = 1;
        return T;
    }
    if(X == T->element){
        fprintf(fout, "insertion error : %d is already in the tree\n", T->element);
    }
    if (X < T->element) {
        T->left = Insert(X, T->left);
    } else if (X > T->element) {
        T->right = Insert(X, T->right);
    }
    return rebalance(T);
}

AVLNode* Delete(Element X, AVLNode* T) {
    //예외)삭제할 값이 없는 경우
    if (T == NULL) {
        fprintf(fout, "deletion error : %d is not in the tree\n", X);
        return NULL;
    }
    //자식의 수에 따라 BST와 같은 방법으로 재귀적으로 삭제
    if (X > T->element) {
        T->right = Delete(X, T->right);
    } else if (X < T->element) {
        T->left = Delete(X, T->left);
    } else {
        if (T->left == NULL || T->right == NULL) {
            AVLNode* tmp = T->left ? T->left : T->right;
            if (tmp == NULL) {
                tmp = T;
                T = NULL;
            } else {
                *T = *tmp;
            }
            free(tmp);
        } else {
            AVLNode* tmp = T->right;
            while (tmp->left != NULL) {
                tmp = tmp->left;
            }
            T->element = tmp->element;
            T->right = Delete(tmp->element, T->right);
        }
    }
    if (T == NULL) {
        return T;
    }
    return rebalance(T);
}

void PrintInorder(AVLNode* T) {
    //left->node->right 순으로 element 출력
    if (T == NULL) {
        return;
    }
    if (T->left != NULL) {
        PrintInorder(T->left);
    }
    fprintf(fout, "%d(%d) ", T->element, T->height);
    if (T->right != NULL) {
        PrintInorder(T->right);
    }
}

void DeleteTree(AVLNode* T) {
    //left->right->node순으로 메모리 해제
    if (T == NULL) {
        return;
    }
    if (T->left != NULL) {
        DeleteTree(T->left);
    }
    if (T->right != NULL) {
        DeleteTree(T->right);
    }
    free(T);
}

int main(int argc, char* argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    AVLNode* Tree = NULL;
    char cv;
    int key;

    while (!feof(fin)) {
        fscanf(fin, "%c", &cv);
        switch (cv) {
            case 'i':
                fscanf(fin, "%d\n", &key);
                Tree = Insert(key, Tree);
                break;
            case 'd':
                fscanf(fin, "%d\n", &key);
                Tree = Delete(key, Tree);
                break;
        }
        PrintInorder(Tree);
        fprintf(fout, "\n");
    }

    DeleteTree(Tree);
    fclose(fin);
    fclose(fout);

    return 0;
}
