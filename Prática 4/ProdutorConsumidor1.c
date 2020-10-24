#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LancaErro.c"

sem_t mutexAccessB1;//mutex para buffer 1
sem_t mutexAccessB2;//mutex para buffer 2
sem_t qtdItemsBuffer1;//controla a quantidade de itens no buffer 1
sem_t qtdItemsBuffer2;//controla a quantidade de itens no buffer 2
sem_t qtdVagasBuffer1;//controla a quantidade de vagas no buffer 1
sem_t qtdVagasBuffer2;//controla a quantidade de vagas no buffer 2

void insere(char * paraImprimir) {
    sleep(1);
    printf("%s\n", paraImprimir);  
}
void retira(char * paraImprimir) {
    sleep(1);
    printf("%s\n", paraImprimir);  
}
void * taskX(void * args) {
    //O processo X insere em B1
    while(1) {        
        sem_wait(&qtdVagasBuffer1);
        sem_wait(&mutexAccessB1);
        //insere item
        insere("item inserido no buffer 1 por X");
        sem_post(&mutexAccessB1);
        sem_post(&qtdItemsBuffer1);
    }
}

void * taskY(void * args) {
    //O processo Y consome de B1 e insere em B2
    while(1) {
        //Primeiro consome um item do buffer 1
        sem_wait(&qtdItemsBuffer1);
        sem_wait(&mutexAccessB1);
        retira("item retirado do buffer 1 por Y");
        sem_post(&mutexAccessB1);
        sem_post(&qtdVagasBuffer1);

        //Agora insere no buffer 2
        sem_wait(&qtdVagasBuffer2);
        sem_wait(&mutexAccessB2);
        insere("item inserido no buffer 2 por Y");
        sem_post(&mutexAccessB2);
        sem_post(&qtdItemsBuffer2);
    }
}

void * taskZ(void * args) {
    //O processo Z consome de B2
    while(1) {
        sem_wait(&qtdItemsBuffer2);
        sem_wait(&mutexAccessB2);
        retira("item retirado do buffer 2 por Z");
        sem_post(&mutexAccessB2);
        sem_post(&qtdVagasBuffer2);
    }
}

int main(int argc, char ** args) {

    //get args: size of buffer 1 and size of buffer 2
    if (argc != 3) {
        printf("Erro: argumentos inválidos. Informe inteiros N1 > 0 e N2 > 0\n");
        exit(1);
    }

    int n1 = atoi(args[1]);
    int n2 = atoi(args[2]);

    //test arguments
    if (n1 <= 0 || n2 <= 0) {
        printf("Erro: argumentos inválidos. Informe inteiros N1 > 0 e N2 > 0\n");
        exit(1);
    }
    
    //init the semaphores
    /*sem_init(
    *   pointer to semaphore you want to init,
    *   0 if you want this semaphore to be ahared between threads,
    *   value you want to init semaphore with
    */
    int semReturnTest = sem_init(&mutexAccessB1, 0, 1);
    semReturnTest = sem_init(&mutexAccessB2, 0, 1);
    semReturnTest += sem_init(&qtdItemsBuffer1, 0, 0);
    semReturnTest += sem_init(&qtdItemsBuffer2, 0, 0);
    semReturnTest += sem_init(&qtdVagasBuffer1, 0, n1);
    semReturnTest += sem_init(&qtdVagasBuffer2, 0, n2);
    
    //test if error ocourred with sem_init
    if (semReturnTest != 0) {
        lancaErro("Não foi possível iniciar semáforos", errno);
    }
    
    //start threads
    pthread_t threadX;
    pthread_t threadY;
    pthread_t threadZ;
    int errTest = pthread_create(&threadX, NULL, taskX, NULL);
    errTest += pthread_create(&threadY, NULL, taskY, NULL);
    errTest += pthread_create(&threadZ, NULL, taskZ, NULL);
    if (errTest != 0){
        lancaErro("Não foi possível criar threads", errno);
    }

    //join threads
    errTest = pthread_join(threadX, NULL);
    errTest += pthread_join(threadY, NULL);
    errTest += pthread_join(threadZ, NULL);
    if (errTest != 0){
        lancaErro("Não foi possível esperar threads criadas", errno);
    }
    return 0;
}