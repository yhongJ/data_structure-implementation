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
    if(Find(header, inserted_key) != NULL){
        fprintf(fout, "insertion %d failed: the key already exists\n", inserted_key);
        return;
    }
    if(target_key == -1){
        LE* new_header = (LE*)malloc(sizeof(LE));

        new_header->key = inserted_key;
        new_header->next_pointer = header->next_pointer;
        header->next_pointer = new_header;
        return;
    }
    if(Find(header, target_key) == NULL){
        fprintf(fout,"insertion %d failed : can not find location\n", inserted_key);
        return;
    }
    header = Find(header, target_key);
    LE* new_header =(LE*)malloc(sizeof(LE));
    
    new_header->key = inserted_key;
    new_header->next_pointer = header->next_pointer;

    header->next_pointer = new_header;
    return;
}

void Delete(LE* header, int target_key){
    if(Find(header, target_key) == NULL){
        fprintf(fout,"deletion %d failed : node is not in the list\n", target_key);
        return;
    }
    header = FindPrevious(header, target_key);
    LE* target_node = header->next_pointer;
    header->next_pointer = target_node->next_pointer;
    free(target_node);
    return;
}


LE* Find(LE* header, int target_key){
    LE* tmp = header;
    
    while(tmp!= NULL){
        if(tmp->key == target_key){
            return tmp;
        }
        tmp = tmp->next_pointer;
    }
    return NULL;
}

LE* FindPrevious(LE* header, int target_key){

    LE* current = header;

    while(current->next_pointer != NULL){
        if(current->next_pointer->key == target_key){
            return current;
        }
        current = current->next_pointer;
    }

    return NULL;
}

void PrintPrevious(LE* header, int target_key){
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
    LE* header = (LE*)malloc(sizeof(LE));

    header->next_pointer = NULL;
    return header;
}

void DeleteList(LE* header){
    LE* current = header;
    while(current->next_pointer!=NULL){
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