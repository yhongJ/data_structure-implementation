#include<stdio.h>
#include<stdlib.h>

FILE* fin;
FILE* fout;

typedef struct Array Array;

struct Array{
    int size;
    int* values;
};

Array* CreateArray(int size);
void QuickSort(Array* array, int left, int right, int depth);
int Partition(Array* array, int left, int right, int depth);
void PrintArray(Array* array, int left, int right);
void DeleteArray(Array* array);
void swap(int* a, int* b);

int main(int argc, char *argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");

    int size, i;
    Array* array;

    fscanf(fin, "%d", &size);
    array = CreateArray(size);
    for(i = 0; i < size; i++){
        fscanf(fin, "%d", &array->values[i]);
    }

    QuickSort(array, 0, size - 1, 0);

    fprintf(fout, "\nsorting result :\n");
    PrintArray(array, 0, size - 1);

    DeleteArray(array);


	return 0;
}

Array* CreateArray(int size){
    //Array 메모리 할당 후 values 메모리 할당
    //Array->size 는 size로 initialize
    Array* array = (Array*)malloc(sizeof(Array));
    array->values = (int*)malloc(sizeof(int) * size);
    array->size = size;
    return array;
}

void QuickSort(Array* array, int left, int right, int depth){

    if(left < right){
        //partition
        int q = Partition(array, left, right, depth);
        //pivot을 기준으로 partition된 두부분을 재귀적으로 quick sort
        QuickSort(array, left, q - 1, depth + 1);
        QuickSort(array, q + 1, right, depth + 1);
    }

}

int Partition(Array* array, int left, int right, int depth){
    //pivot은 배열의 가장 오른쪽 값
    int pivot = array->values[right];
    int i = left - 1;
    fprintf(fout, "left : %d, right : %d, pivot : %d\n", left, right, pivot);
    PrintArray(array, left, right);

    //i는 시작 idx- 1부터, j는 pivot제외 마지막 idx부터 시작
    for (int j = right - 1; j > i; j--) {
        //만약 j의 value가 pivot보다 작은 경우 -> i의 value와 swap해줘야 하는 경우
        if (array->values[j] < pivot) {
            //i의 value가 pivot보다 작으면 j의 value와 바꾸는 의미가 없으므로 i의 value가 pivot보다 클 때까지 i++
            while (array->values[i + 1] < pivot){
                i++;
            }
            if(i >= j) break;
            i++;
            //i 조정 끝난 후 swap
            swap(&array->values[i], &array->values[j]);
            PrintArray(array, left, right);
        }
    }
    //마지막으로 pivot을 적절한 위치 (i + 1)에 삽입 후 그 idx를 return
    swap(array->values + i + 1, array->values + right);
    PrintArray(array, left, right);
    return i + 1;

}

void PrintArray(Array* array, int left, int right){
    int i;
    for(i = 0; i < array->size; i++){
        if(i < left || right < i){
            fprintf(fout, "%-3s", "-");
        }
        else{
            fprintf(fout, "%-3d", array->values[i]);
        }
    }
    fprintf(fout, "\n");
    return;
}

void DeleteArray(Array* array){
    //int* type values부터 메모리 해제 후 array 메모리 해제
    free(array->values);
    free(array);
}

void swap(int* a, int* b) {
    //call-by-reference swap
    int tmp = *a;
    *a = *b;
    *b = tmp;
}
