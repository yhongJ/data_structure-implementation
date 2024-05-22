#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _QueueNode QueueNode;

struct _QueueNode{
    QueueNode* next;
    int data;
};

typedef struct _Queue {
	QueueNode* front;
	QueueNode* rear;
	int size;
}Queue;

typedef struct _Graph {
	int size;
	int* node;
	bool** matrix;
}Graph;

/*
구현해야 하는 함수들
*/
Graph* CreateGraph(int* nodes, int n);

void InsertEdge(Graph* G, int a, int b);

void PrintGraph(Graph* G);

void DeleteGraph(Graph* G);

void TopologicalSort(Graph* G);

Queue* CreateQueue();

bool IsEmpty(Queue* Q);
int Dequeue(Queue* Q);
void Enqueue(Queue* Q, int X);
void DeleteQueue(Queue* Q);

void SetIndegree(Graph* G, int* idg);

void SortNode(int* arr, int n);



void countInput(int* n, char* str) {
	int len = strlen(str), i;
	for (i = 0; i < len; i++)
		if (0 <= str[i] - '0' && str[i] - '0' < 10) (*n)++;
}

void parseInput(int* arr, char* str, int n) {
	int len = strlen(str), i;
	int cnt = 0;
	for (i = 0; i < len; i++)
	if (0 <= str[i] - '0' && str[i] - '0' < 10) arr[cnt++] = str[i] - '0';
}

int main(int argc, char* agrv[]) {
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");
    
	char numbers[100], inputs[100];
	fgets(numbers, 100, fin);
	int n = 0;
	countInput(&n, numbers);
	int* nodes = (int*)malloc(sizeof(int)*n);
	parseInput(nodes, numbers, n);
	
    SortNode(nodes, n);
	Graph* G = CreateGraph(nodes, n);

	fgets(inputs, 100, fin);
	int len = strlen(inputs), i, j;
	for (i = 0; i < len; i += 4) {
		int a = inputs[i] - '0', b = inputs[i + 2] - '0';
		InsertEdge(G, a, b);
	}

	PrintGraph(G);
    fprintf(fout, "\n");
	TopologicalSort(G);
	
	DeleteGraph(G);

	fclose(fin);
	fclose(fout);

	return 0;
}


Graph* CreateGraph(int* nodes, int n){
    //graph 초기화
    //nodes를 graph->node로 할당
    //graph->size는 n
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = n;
    graph->node = (int*)malloc(sizeof(int) * (graph->size));
    for(int i = 0; i < n; i++){
        graph->node[i] = nodes[i];
    }
    graph->matrix = (bool**)malloc(sizeof(bool*) * n);
    for(int i = 0; i < n; i++ ){
        graph->matrix[i] = (bool*)malloc(sizeof(bool) * n);
        for(int j = 0; j  < n; j++){
            graph->matrix[i][j]  = 0;
        }
    }
    return graph;
    
}

void InsertEdge(Graph* G, int a, int b){
    //for문을 통해 G->node에서 a, b의 인덱스를 찾은뒤 G->matrix에 edge추가
    //matrix[i][j]에서 direction은 i->j

    int idx_a, idx_b;

    for(int i = 0; i < G->size; i++){
        if(G->node[i] == a) idx_a = i;
        if(G->node[i] == b) idx_b = i;
    }

    G->matrix[idx_a][idx_b] = 1;
    return;
   
    
}

void PrintGraph(Graph* G){
    int n = G->size;

    fprintf(fout, "%d ", 0);

    //첫 행과 첫 열은 node의 값들을 출력
    for(int i = 0; i < n; i++){
        fprintf(fout, "%d ", G->node[i]);
    }
    fprintf(fout, "\n");
    for(int i = 0; i < n; i++){
        fprintf(fout, "%d ", G->node[i]);
        for(int j = 0; j < n; j++){
            //matrix를 통해 edge유무를출력 (있으면 1, 없으면 0)
            fprintf(fout, "%d ", G->matrix[i][j]);
        }
        fprintf(fout, "\n");

        
    }
    return;
    
}

void DeleteGraph(Graph* G){
    //G->node 메모리 해제
    free(G->node);
    //G->matrix는 2차원 배열이므로 for문을 통해 행마다 메모리 해제
    for(int i = 0; i < G->size; i++){
        free(G->matrix[i]);
    }
    //최종적으로 G메모리 해제
    free(G);
    return;
    
}

void TopologicalSort(Graph* G){
    
    int n = G->size;
    //print_size: dequeue된 값들의 갯수-> n과 같지 않으면 cycle존재
    int print_size = 0;
    //printOrder: 알고리즘 순서대로 dequeue된 node값들
    int* printOrder = (int*)malloc(sizeof(int) * 10000);

    Queue* queue = CreateQueue();

    int* indegree = (int*)malloc(sizeof(int) * (n));
    SetIndegree(G, indegree);
    //indgree가 0이면 enqueue
    for(int i = 0; i < n; i++){
        if(indegree[i] == 0){
            Enqueue(queue, i);
        }
    }
    //queue가 empty할때까지 topological sort적용
    //dequeue하는 값(current)와 연결된 노드의 indegree는 1씩 감소 (indgree가 0이 되면 enequeue)
    while(!IsEmpty(queue)){
        int current = Dequeue(queue);
        printOrder[print_size] = G->node[current];
        print_size++;
        for(int j = 0; j < n; j++){
            if(G->matrix[current][j]){
                indegree[j]--;
                if(indegree[j] == 0){
                    Enqueue(queue, j);
                }
            }
        }
        
    }
    //print_size와 graph의 크기가 다르면 cycle존재한다고 판단->error message 출력
    if(print_size != n){
        fprintf(fout, "sorting error : cycle\n");
        return;
        }
    else{
        for(int i = 0; i < n; i++){
            fprintf(fout, "%d ", printOrder[i]);
        }
        }
    //indgre, printOrder 메모리해제, queue는 DeleteQueue 이용해서 메모리해제
    free(indegree);
    free(printOrder);
    DeleteQueue(queue);


}

Queue* CreateQueue(){

    //queue메모리 할당. 처음엔 front, rear은 모두 NULL로 초기화

    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->size = 0;
    queue->front = queue->rear = NULL;
    return queue;
 
}

bool IsEmpty(Queue* Q){
    //queue의 사이즈가 0이면 1을 return
    if(!Q->size) return 1;
    return 0;
}

int Dequeue(Queue* Q){

    int value = Q->front->data;
    QueueNode* front = Q->front;
    Q->front = Q->front->next;
    Q->size--;
    if(Q->size == 0){
        Q->front = Q->rear = NULL;
    }
    free(front);
    
    //queue 맨 앞쪽의 노드를 꺼내기
    //size --
    //linked list때와 동일하게 Q->front 포인터를 갱신(빼면 비게될 경우 front, rear 둘 다 NULL로)
    return value;
}

void Enqueue(Queue* Q, int X){
    QueueNode* queueNode = (QueueNode*)malloc(sizeof(QueueNode));
    queueNode->data = X;
    queueNode->next = NULL;

    if(IsEmpty(Q)){
        //만약 Q가 비어있다면 front와 rear 모두 queueNode로 설정
        Q->front = Q->rear = queueNode;
        Q->size++;
        return;
        
    }
    else{
        //기존 rear의 next를 queueNode로 설정
        //Q->rear을 queueNode로 업데이트
        Q->rear->next = queueNode;
        Q->rear = queueNode;
        Q->size++;
        return;
    }
    
}

void DeleteQueue(Queue* Q){
    //Q가 비어있을 때까지 Deque후 free
    while(!IsEmpty(Q)){
        Dequeue(Q);
    }
    free(Q);
    return;

}

void SetIndegree(Graph* G, int* idg){
    int n = G->size;
    //우선 모든 idg값 0으로 초기화
    for(int i = 0; i < n; i++){
        idg[i] = 0;
    }
    //matrix[i][j] = 1이라면 indegree[j] ++
    for(int i = 0; i < n; i++){
        for(int j = 0; j < n; j++){
            if(G->matrix[i][j]) idg[j]++;
        }
    }
    return;

}

void SortNode(int* arr, int n){

    //node를 오름차순으로 정렬

    for(int i = 0; i < n; i++){
        for(int j = i; j < n; j++){
            if(arr[i] > arr[j]){
                int tmp = arr[i];
                arr[i] = arr[j];
                arr[j] = tmp;
            }
        }
    }

    return;
}

