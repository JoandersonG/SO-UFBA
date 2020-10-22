#include <pthread.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

char* buffer1;
char* buffer2;

void insere(int i){
    printf("item foi inserido no buffer 𝐵%i", i);
}

void retira(int i){
    printf("item foi retirado no buffer 𝐵%i", i);
}

int main(){
    insere(1);
    retira(2);
}