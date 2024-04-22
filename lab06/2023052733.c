#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>
#define INF 1e9

FILE *fin;
FILE *fout;

typedef struct HeapStruct{
	int Capacity;
	int Size;
	int *Elements;
}Heap;

Heap* CreateHeap(int heapSize);
void DeleteHeap(Heap* heap);
void Push(Heap *heap, int value);
bool Find(Heap *heap, int value);
int Pop(Heap* heap);
void PrintHeap(Heap* heap);

Heap* CreateHeap(int heapSize){
    //heap malloc, elements malloc, capcaity 및 size초기화
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->Capacity = heapSize;
    heap->Elements = (int*)malloc(sizeof(int) * heap->Capacity);
    heap->Size= 0;
    return heap;
    
}
void DeleteHeap(Heap* heap){
    //int*형 elements 메모리 해제 후 heap메모리 해제
    free(heap->Elements);
    free(heap);
}
void Push(Heap *heap, int value){
    //size 1 증가, elements의 마지막 인덱스에 value삽입
    heap->Size++;
    heap->Elements[heap->Size - 1] =  value;
    //idx: value의 현재 인덱스
    int idx = heap->Size - 1;
    //부모노드와 비교해가며 부모가 자신보다 클 때까지 swap반복
    while(1){
        if(heap->Elements[idx] > heap->Elements[(idx - 1) / 2] ){
            int tmp = heap->Elements[idx];
            heap->Elements[idx] = heap->Elements[(idx - 1) / 2];
            heap->Elements[(idx - 1) / 2]  = tmp;
            idx = (idx - 1) / 2;
        }
        else{
            break;
        }
    }
    return;
    
}
bool Find(Heap *heap, int value){
    //인덱스 0부터 순차 탐색
    for(int i = 0; i < heap->Size; i++){
        if(heap->Elements[i] == value){
            return true;
        }
    }
    return false;
}
int Pop(Heap* heap){
    if(heap->Size == 0){
        return -INF;
    }
    int tmp = heap->Elements[0];
    //삭제할 값과 마지막 인덱스 값 swap
    heap->Elements[0] = heap->Elements[heap->Size - 1];
    heap->Elements[heap->Size - 1] = tmp;
    //size -1
    heap->Size--;
    //마지막 인덱스의 값 삭제->-INF로 설정
    heap->Elements[heap->Size] = -INF;
    int idx = 0;
    while(1){
        //percolate down.아래로 탐색해가며 두 자식중 더큰 것과 비교하여 부모가 더 작으면 swap반복
        int largest = idx;
        int leftIdx = 2 * idx + 1;
        int rightIdx = 2 * idx + 2;

        if (leftIdx < heap->Size && heap->Elements[leftIdx] > heap->Elements[largest]) {
            largest = leftIdx;
        }
        if (rightIdx < heap->Size && heap->Elements[rightIdx] > heap->Elements[largest]) {
            largest = rightIdx;
        }

        if (largest != idx) {
            int temp = heap->Elements[idx];
            heap->Elements[idx] = heap->Elements[largest];
            heap->Elements[largest] = temp;
            idx = largest;
        } else {
            break;
        }
    }
    //tmp에 저장해둔 삭제할 값 return
    return tmp;
    }


void PrintHeap(Heap* heap){
    //예외 처리: szie가 0이면 empty임을 알림
    if(heap->Size == 0){
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    //인덱스 0부터 순차 출력
    for(int i = 0; i < heap->Size; i++){
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
    return;

}

int main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

	char cv;
	Heap* maxHeap;
	int heapSize, key, max_element;

	while(!feof(fin)){
		fscanf(fin, "%c", &cv);
		switch(cv){
			case 'n':
				fscanf(fin, "%d", &heapSize);
				maxHeap = CreateHeap(heapSize);
				break;
			case 'i':
				fscanf(fin, "%d", &key);
                if(maxHeap->Capacity == maxHeap->Size){
                    fprintf(fout, "push error : heap is full\n");
                    break;
                }
                if(Find(maxHeap, key)){
                    fprintf(fout, "push error : %d is already in the heap\n", key);
                    break;
                }
            
				Push(maxHeap, key);
				break;
			case 'd':
				max_element = Pop(maxHeap);
				if(max_element != -INF){
					fprintf(fout, "max element : %d deleted\n", max_element);
				}
                else{
                    fprintf(fout,"pop error : heap is empty\n");
                }
				break;
			case 'p':
				PrintHeap(maxHeap);
				break;
			case 'f':
				fscanf(fin, "%d", &key);
				if(Find(maxHeap, key)) fprintf(fout, "%d is in the heap\n", key);
				else fprintf(fout, "%d is not in the heap\n", key);
				break;
		}
	}
	DeleteHeap(maxHeap);

	return 0;
}