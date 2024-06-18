#include<stdio.h>
#include<stdlib.h>
#include<time.h>
#include<stdbool.h>

FILE *fin;
FILE *fout;

typedef struct _DisjointSet {
	int size_cell;
	int* ptr_cell;
	int size_wall;
	bool* ptr_wall;
}DisjointSets;

DisjointSets* init(int num);
void Union(DisjointSets *sets, int i, int j);
int find(DisjointSets *sets, int i);
void createMaze(DisjointSets *sets, int num);
void printMaze(DisjointSets *sets, int num);
void freeMaze(DisjointSets *sets);

void swap(int *x, int *y) {
    int tmp = *x;
    *x = *y;
    *y = tmp;
}

int main(int argc, char* agrv[]) {
	srand((unsigned int)time(NULL));

	int num, i;
	fin = fopen(agrv[1], "r");
	fout = fopen(agrv[2], "w");

	fscanf(fin, "%d", &num);

	DisjointSets* sets = init(num);
	createMaze(sets, num);
	printMaze(sets, num);

	freeMaze(sets);

	fclose(fin);
	fclose(fout);

	return 0;
}

DisjointSets* init(int num) {
	//cell의 인덱스: 0 ~ num^2 - 1
	//wall의 인덱스: 0 ~ 2 * num^2 - 1 (i 번째 셀의 우측은 i*2, 하단은 i*2+1로 정의)
	int size_cell = num * num;
	int size_wall = size_cell * 2;
	DisjointSets* disjointsets = (DisjointSets*)malloc(sizeof(DisjointSets));
	disjointsets->size_cell = size_cell;
	disjointsets->ptr_cell = (int*)malloc(sizeof(int) * size_cell);

	//ptr_cell은 cell들의 disjoint set을 나타내는 array. 현재는 모든 cell들이 disjoint하므로 자기 자신을 value로 가짐
	//만약 인덱스와 value가 다르다면 그 value는 disjoint set의 inverted tree에서의 parent
	for(int i = 0; i < size_cell; i++) disjointsets->ptr_cell[i] = i;


	disjointsets->size_wall = size_wall;
	disjointsets->ptr_wall = (bool*)malloc(sizeof(bool) * disjointsets->size_wall);
    //처음엔 모든 벽이 존재하므로 true로 초기화
	for(int i = 0; i < size_wall; i++) disjointsets->ptr_wall[i] = true;
	//다만 출구만 false로 지정
	disjointsets->ptr_wall[size_wall - 2] = false;

	return disjointsets;
}

void Union(DisjointSets *sets, int i, int j) {

	//i와 j의 set의 대푯값(각각 pi, pj)들을 찾아 pi->pj 방향으로 Union
	int pi = find(sets, i);
	int pj = find(sets, j);
	sets->ptr_cell[pi] = pj;        

	return;
}

int find(DisjointSets *sets, int i) {	

	//재귀적으로 disjoint set의 array의 value가 인덱스와 같을 때까지 find하여 대푯값을 return
	if(sets->ptr_cell[i] == i ){
		return i;
	}

    return find(sets, sets->ptr_cell[i]);
}

void shuffle(int* array, int num){

	//0~num까지 반복하며 현재 인덱스 i와 randint를 swap하는걸로 shuffle함수를 구현
	
	for(int i = 0; i < num; i++){
		int randint = rand() % num;
		swap(&array[i], &array[randint]);
	}
}

void createMaze(DisjointSets *sets, int num) {
	int size = num * num;
	int* arr = (int*)malloc(sizeof(int) * size);
	for(int i = 0; i < size; i++) arr[i] = i;
	shuffle(arr, size);
	for(int i = 0; i < size; i++){
		int tmp = arr[i];
		//tmp가 맨 오른쪽 cell이 아니고 (오른쪽 벽 제거 가능) 오른쪽 cell과 disjoint 할 때 오른쪽 벽을 제거
		if((tmp + 1) % num != 0 && find(sets, sets->ptr_cell[tmp]) != find(sets, sets->ptr_cell[tmp + 1])){
			Union(sets, tmp, tmp + 1);
			sets->ptr_wall[2 * tmp] = false;
			}
		//tmp가 맨 아래 cell이 아니고 (아래쪽 벽 제거 가능) 아래쪽 cell과 disjoint 할 때 아래쪽 벽을 제거
		if(tmp + num < size && find(sets, sets->ptr_cell[tmp]) != find(sets, sets->ptr_cell[tmp + num])){
			Union(sets, tmp, tmp + num);
			sets->ptr_wall[2 * tmp + 1] = false;
					
		}		
	}
}

void printMaze(DisjointSets *sets, int num) {

	//맨 첫줄 출력
    for (int i = 0; i < num; i++) fprintf(fout, "+---");
	
	fprintf(fout, "+\n");

	//num번 반복하며 수직벽 -> 수평벽 차례로 출력
    for(int i = 0; i < num; i++){

		//i == 0인 경우만 예외로 입구가 뚤려 있으므로 case 분리
		if(i == 0) fprintf(fout, "    ");
		else fprintf(fout, "|   ");

		//수직벽
		for(int j = 0; j < num; j++){
			//ptr_wall이 참이면 수직벽을 출력
			if(sets->ptr_wall[2 * i * num + 2 * j]){
				fprintf(fout, "|   ");
			}
			//거짓이면 공백 출력
			else{
				fprintf(fout, "    ");
			}
		}
		
		fprintf(fout, "\n");
		//수평벽
		for(int j = 0; j < num; j++){
			//ptr_wall이 참이면 cell(+)과 수평벽을 출력
			if(sets->ptr_wall[2 * i * num + 2 * j + 1]){
				fprintf(fout, "+---");
			}
			//거짓이면 cell과 공백을 출력
			else{
				fprintf(fout, "+   ");
			}
		}
		fprintf(fout, "+\n");
		
	}


	}


void freeMaze(DisjointSets *sets) {
	free(sets->ptr_cell);
    free(sets->ptr_wall);
    free(sets);
    //메모리 free
}
