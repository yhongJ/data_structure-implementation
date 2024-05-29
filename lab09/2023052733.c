#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define INF 1e7

FILE *fin;
FILE *fout;

typedef struct _Node {
    int index;  
    int dist;   
    int prev;
} Node;

typedef struct _Graph {
    int size;
    int **distMatrix;
    Node **nodes;
} Graph;

typedef struct _Heap {
    int capacity;
    int size;
    Node **element;
} Heap;


Graph* CreateGraph(int size);
void DeleteGraph(Graph* g);
void PrintShortestPath(Graph* g);
Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap* heap, Node* node);
Node* Pop(Heap* heap);
bool isEmpty(Heap* heap);
void swap(Node** node1, Node** node2);

bool isEmpty(Heap* heap) {
    //heap의 사이즈가 0이면 1, 아니면 0을 return
    return heap->size == 0;
}

void swap(Node** node1, Node** node2) {
    Node* tmp = *node1;
    *node1 = *node2;
    *node2 = tmp;
}

int main(int argc, char *argv[]) {
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");

    Graph* g;
    int size;
    fscanf(fin, "%d\n", &size);
    g = CreateGraph(size);

    char tmp = 0;

    while (tmp != '\n' && tmp != EOF) {
        int node1, node2, weight;
        fscanf(fin, "%d-%d-%d", &node1, &node2, &weight);
        g->distMatrix[node1 - 1][node2 - 1] = weight;
        tmp = fgetc(fin);
    }

    PrintShortestPath(g);

    DeleteGraph(g);
	
    return 0;
}

Graph* CreateGraph(int size) {
    Graph* graph = (Graph*)malloc(sizeof(Graph));
    graph->size = size;

    //다익스트라 알고리즘은 음수 거리를 처리할 수 없으므로 초기값을 -1로 설정 (= 길이 없음을 의미)
    graph->distMatrix = (int**)malloc(size * sizeof(int*));
    for (int i = 0; i < size; i++) {
        graph->distMatrix[i] = (int*)malloc(size * sizeof(int));
        for(int j = 0; j < size; j++){
            graph->distMatrix[i][j] = -1;

        }
    }

    //인덱스는 1부터 시작 ->i + 1
    //dist와 prev는 무한대로 초기화 
    graph->nodes = (Node**)malloc(size * sizeof(Node*));
    for (int i = 0; i < size; i++) {
        graph->nodes[i] = (Node*)malloc(sizeof(Node));
        graph->nodes[i]->index = i + 1;
        graph->nodes[i]->dist = INF;
        graph->nodes[i]->prev = INF;
    }

    return graph;
}

void DeleteGraph(Graph* graph) {
    for (int i = 0; i < graph->size; i++) {
        free(graph->distMatrix[i]);
        free(graph->nodes[i]);
    }
    //이중 포인터인 disMatrix와 nodes들은 for문을 통해 free후 free
    free(graph->distMatrix);
    free(graph->nodes);
    //마지막으로 graph free
    free(graph);
}

Heap* CreateHeap(int heapSize) {
    //heap->capacity를 heapsize, size를 0으로 초기화
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->capacity = heapSize;
    heap->size = 0;
    heap->element = (Node**)malloc(heapSize * sizeof(Node*));
    return heap;
}

void DeleteHeap(Heap* heap) {
    //heap의 element를 for문을 통해 free
    for(int i = 0; i < heap->size; i++){
        free(heap->element[i]);
    }
    free(heap->element);
    //마지막에 heap을 free
    free(heap);
}

void Push(Heap* heap, Node* node) {
    if (heap->size == heap->capacity) {
        return; 
        //heap이 차있는 경우 예외적으로 처리
    }
    heap->size++; //노드 하나를 push하므로 size를 ++
    int idx = heap->size - 1;
    //idx의 부모 노드의 인덱스는 (idx - 1) / 2
    //다익스트라의 heap은 Min heap이므로 node->dist가 부모 노드보의 distance보다 작을 때까지 반복
    while (idx > 0 && node->dist < heap->element[(idx - 1) / 2]->dist) { 
        //idx의 값을 부모의 값으로 교체->노드를 하나씩 내리는 작업
        heap->element[idx] = heap->element[(idx - 1) / 2];
        idx = (idx - 1) / 2;
    }
    //조건문이 끝난 후 idx에 노드를 삽입
    heap->element[idx] = node;
}

Node* Pop(Heap* heap) {
    if (heap->size <= 0) {
        return NULL;
    }
    if (heap->size == 1) {
        heap->size--;
        return heap->element[heap->size];
    }

    Node* root = heap->element[0];
    heap->element[0] = heap->element[--heap->size];
    int i = 0;
    //i의 자식노드가 heap의 사이즈 안에 있는 동안 조건문을 실행
    while ((2 * i + 1) < heap->size) {
        int smallest = i;
        int leftIdx = 2 * i + 1;
        int rightIdx = 2 * i + 2;

        //heap의 pop과정
        //percolate down.아래로 탐색해가며 두 자식중 더 작은 것과 비교하여 부모가 더 크면 swap반복
        if (leftIdx < heap->size && heap->element[leftIdx]->dist < heap->element[smallest]->dist) {
            smallest = leftIdx;
        }
        if (rightIdx < heap->size && heap->element[rightIdx]->dist < heap->element[smallest]->dist) {
            smallest = rightIdx;
        }
        if (smallest == i) {
            break;
        }
        Node* tmp = heap->element[i];
        heap->element[i] = heap->element[smallest];
        heap->element[smallest] = tmp;
        i = smallest;
    }
    return root;
}

void PrintShortestPath(Graph* graph) {
    //시작 노드의 거리는 0으로 시작
    graph->nodes[0]->dist = 0; 

    Heap* heap = CreateHeap(graph->size);
    Push(heap, graph->nodes[0]); //시작 노드를 heap에 push

    while (!isEmpty(heap)) {
        //heap에서 pop된 노드를 current로 사용
        Node* current = Pop(heap);
        int currentIdx = current->index - 1;

        for (int i = 0; i < graph->size; i++) {
            //만약 i가 currentIdx가 아니고 disMatrix값이 명시된 경우 (= 길이 있는 경우) 조건문을 실행
            if (i != currentIdx && graph->distMatrix[currentIdx][i] >= 0) {
                //노드의 dist가 currrent의 dist + current -> node[i] 보다 큰 경우 cost를 업데이트
                if (current->dist + graph->distMatrix[currentIdx][i] < graph->nodes[i]->dist) {
                    graph->nodes[i]->dist = current->dist + graph->distMatrix[currentIdx][i];;
                    graph->nodes[i]->prev = currentIdx;
                    Push(heap, graph->nodes[i]);
                }
            }
        }
    }

    //최단 경로를 출력
    for (int i = 1; i < graph->size; i++) {
    
        //거리가 INF인 경우-> 갈 수있는 경로가 없음
        if(graph->nodes[i]->dist == INF){
            fprintf(fout, "can not reach to node %d\n", i + 1);
            continue;
        }
        //간 경로를 기록하기 위한 path 배열을 선언
        int path[graph->size];
        int cnt = 0;
        int prev = graph->nodes[i]->prev;
        //prev가 INF가 아닌 경우 -> 이전 노드를 path에 저장
        while (prev != INF) {
            path[cnt] = prev + 1;
            prev = graph->nodes[prev]->prev;
            cnt++;
        }
        //시작 노드가 1이므로 우선 1부터 출력
        fprintf(fout, "1");
        //path에는 도착 노드부터 들어가있기 때문에 역순으로 출력
        for (int j = cnt - 2; j >= 0; j--) {
            fprintf(fout, "->%d", path[j]);
        }
        //마지막으로 cost를 출력
        fprintf(fout, "->%d (cost: %d)\n", i + 1, graph->nodes[i]->dist );
    }

    DeleteHeap(heap);
}
