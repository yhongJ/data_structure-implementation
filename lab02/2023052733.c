#include<stdio.h>
#include<stdlib.h>

FILE *fin;
FILE *fout;

typedef struct LinkedListElement LE;

struct LinkedListElement{
    int key;
    LE* next_pointer;
};

void PrintPrevious(LE* header, int target_key);
void PrintList(LE* header);
void Insert(LE* header, int inserted_key, int target_key);
void Delete(LE* header, int target_key);
LE* Find(LE* header, int target_key);
LE* FindPrevious(LE* header, int target_key);

void Insert(LE* header, int inserted_key, int target_key){
    //inserted_key: 넣고자 하는 값, target_key: 넣을 위치의 값
    if(Find(header, inserted_key) != NULL){
        //inserted_key가 이미 있는 경우
        fprintf(fout, "insertion %d failed: the key already exists\n", inserted_key);
        return;
    }
    if(target_key == -1){
        LE* new_header = (LE*)malloc(sizeof(LE));
        //target_key가 1인 경우: header의 다음 위치에 연결
        new_header->key = inserted_key;
        new_header->next_pointer = header->next_pointer;
        header->next_pointer = new_header;
        return;
    }
    if(Find(header, target_key) == NULL){
        //target_key를 찾지 못한 경우
        fprintf(fout,"insertion %d failed : can not find location\n", inserted_key);
        return;
    }
    header = Find(header, target_key);
    LE* new_header =(LE*)malloc(sizeof(LE));
    //inserted_key를 값으로 가지는 new_header을 만들어서 next_pointer에 연결 후 이전 노드의 next_point에 new_header을 연결
    new_header->key = inserted_key;
    new_header->next_pointer = header->next_pointer;

    header->next_pointer = new_header;
    return;
}

void Delete(LE* header, int target_key){
    //target_key: 삭제할 값
    if(Find(header, target_key) == NULL){
        //삭제할 값을 찾지 못한 경운
        fprintf(fout,"deletion %d failed : node is not in the list\n", target_key);
        return;
    }
    //target_node:삭제할 노드
    //이전 노드를 찾아서 next_pointer을 target_node의 next_pointer로 연결 (target_node그 자체로 연결하면 안됨. 삭제해야하므로)
    header = FindPrevious(header, target_key);
    LE* target_node = header->next_pointer;
    header->next_pointer = target_node->next_pointer;
    free(target_node);
    return;
}


LE* Find(LE* header, int target_key){
    //current노드를 선언해서 target_key를 찾을때까지 next_pointer로 이어가며 탐색
    LE* current = header;
    
    while(current!= NULL){
        if(current->key == target_key){
            return current;
        }
        current = current->next_pointer;
    }
    //만약 찾지 못한 경우 NULL을 return
    return NULL;
}


//Delete할 때에는 이전 노드를 알아야하므로 Delete를 위해 FindPrevious함수가 필요함
LE* FindPrevious(LE* header, int target_key){
    //current노드를 선언해서 target_key를 찾을때까지 next_pointer로 이어가며 탐색
    //다만 previous를 찾을 것이므로 Find와는 달리 next_pointer->key가 target_key인 경우를 찾는 것임
    LE* current = header;
    
    while(current->next_pointer != NULL){
        if(current->next_pointer->key == target_key){
            return current;
        }
        current = current->next_pointer;
    }
    //Find와 마찬가지로 찾지 못한 경우 NULL을 return
    return NULL;
}

void PrintPrevious(LE* header, int target_key){
    //사용자가 command로 f를 입력한 경우 FindPrevious함수의 결과에 따라 출력
    if(FindPrevious(header, target_key) == NULL){
        fprintf(fout, "finding %d failed : node is not in the list\n", target_key);
        return;
    }
    if(header == FindPrevious(header, target_key)){
        fprintf(fout, "previous of node of %d is head\n", target_key);
        return;
    }
    fprintf(fout, "previous node of %d is %d\n", target_key, FindPrevious(header, target_key)->key);
    return;
}

void PrintList(LE* header){
    //사용자가 command로 p를 입력한 경우 header에서 부터 시작해서 next_pointer을 따라가며 모든 원소를 출력
    if(header->next_pointer == NULL){
        fprintf(fout,"empty list\n");
        return;
    }
    while(header->next_pointer != NULL){
        fprintf(fout,"%d ", header->next_pointer->key);
        header = header->next_pointer;
    }
    fprintf(fout,"\n");
}

LE* CreateList(){
    //처음 linked list를 만들때 header을 선언
    LE* header = (LE*)malloc(sizeof(LE));

    header->next_pointer = NULL;
    return header;
}

void DeleteList(LE* header){
    //memory leak을 막기위해 모든 작업이 끝나면 header에서부터 next_pointer을 따라가며 하나씩 free
    LE* current = header;
    while(current->next_pointer!=NULL){
        //tmp에 next_pointer을 할당해놓고 current를 free, tmp를 current에 할당, 반복
        LE* tmp = current->next_pointer;
        free(current);
        current = tmp;
    }
    free(current);
}

void main(int argc, char* argv[]){
	fin = fopen(argv[1], "r");
	fout = fopen(argv[2], "w");
	char command;
	int key1, key2;

    LE* header = CreateList();
	
	while(1){
		command = fgetc(fin);
		if(feof(fin)) break;
		switch(command){
		    case 'i':
		        fscanf(fin, "%d %d", &key1, &key2);
		        Insert(header, key1, key2);
		        break;
		    case 'd':
		        fscanf(fin, "%d", &key1);
		        Delete(header, key1);
		        break;
		    case 'f':
		        fscanf(fin, "%d", &key1);
		        PrintPrevious(header, key1);
		        break;
		    case 'p':
		        PrintList(header);
		        break;
		}
	}
	DeleteList(header);
	fclose(fin);
	fclose(fout);
	return;
}