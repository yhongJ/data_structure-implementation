#include <stdio.h>
#include <stdlib.h>

#define max(a, b) ((a > b) ? a : b)
#define min(a, b) ((a < b) ? a : b)

FILE* fin;
FILE* fout;

long long sum_between_two(int a, int b){
    int max = max(a, b);
    int min = min(a, b);
    int result = 0;
    for(int i = min; i <= max; i++){
        result += i;
    }
}


void main(int argc, char * argv[]){
    fin = fopen(argv[1], "r");
    fout = fopen(argv[2], "w");
    int a, b;
    long long sum;

    while (fscanf(fin, "%d %d", & a, & b) != EOF) {
        sum = sum_between_two(a, b);
        fprintf(fout, "sum of %d and %d is %lld\n", a, b, sum);
    }
    fclose(fin);
    fclose(fout);
    return;

}