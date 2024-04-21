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
    
    Heap* heap = (Heap*)malloc(sizeof(Heap));
    heap->Capacity = heapSize;
    heap->Elements = (int*)malloc(sizeof(int) * heap->Capacity);
    heap->Size= 0;
    return heap;
    // heap malloc
}
void DeleteHeap(Heap* heap){
    free(heap->Elements);
    free(heap);
    // heap free
}
void Push(Heap *heap, int value){
    heap->Size++;
    heap->Elements[heap->Size - 1] =  value;
    int idx = heap->Size - 1;

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
    // heap->Elements 끝에 value 삽입
    // 반복:
    //     if 부모_value < 현재_value
    //          swap (부모_pointer, 현재_pointer)
    //          현재_pointer = 부모_pointer
}
bool Find(Heap *heap, int value){
    for(int i = 0; i < heap->Size; i++){
        if(heap->Elements[i] == value){
            return true;
        }
    }
    // heap->Elements 에서 값 찾기
    return false;
}
int Pop(Heap* heap){
    if(heap->Size == 0){
        return -INF;
    }
    int tmp = heap->Elements[0];
    heap->Elements[0] = heap->Elements[heap->Size - 1];
    heap->Elements[heap->Size - 1] = tmp;
    heap->Size--;
    heap->Elements[heap->Size] = -INF;
    int idx = 0;
    while(1){
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
    return tmp;
    }

    // heap->Elements[1]을 heap->Elements의 마지막 key와 변경
    // Size 변경
    // 반복:
    //     자식_왼쪽_value = -INF
    //     자식_오른쪽_value = -INF
    //     if 자식_왼쪽_있음
    //         자식_왼쪽_value 설정
    //     if 자식_오른쪽_있음
    //         자식_오른쪽_value 설정
    //
    //     if 자식_왼쪽_value < 자식_오른쪽_value
    //     and 현재_value < 자식_오른쪽_value
    //         swap(현재_pointer, 자식_오른쪽_pointer)
    //         현재_pointer = 자식_오른쪽_pointer
    //     elif 자식_오른쪽_value < 자식_왼쪽_value
    //     and 현재_value < 자식_왼쪽_value
    //         swap(현재_pointer, 자식_왼쪽_pointer)
    //         현재_pointer = 자식_왼쪽_pointer
    // return 삭제한 key

void PrintHeap(Heap* heap){
    if(heap->Size == 0){
        fprintf(fout, "print error : heap is empty\n");
        return;
    }
    for(int i = 0; i < heap->Size; i++){
        fprintf(fout, "%d ", heap->Elements[i]);
    }
    fprintf(fout, "\n");
    return;
    // heap->Elements 순서대로 print
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