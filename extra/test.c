#include <stdio.h>

int num;
int grow();

int main(){
    printf("%d", num);
    grow();
}

int grow(){
        num = 10000;
}
