#include<stdio.h>
#include <errno.h>
#include <semaphore.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include "LancaErro.c"

//int MAX = 100;
sem_t vazios;//gerencia a quantidade de buracos vazios, começa com 0
//sem_t buracos;//gerencia o total de buracos, começa com 0
sem_t afechar;//gerencia a quantidade de buracos prontos pra fechar, começa com 0
sem_t pa;//gerencia o uso da pá, inicia em 0
sem_t MAXburacos;//gerencia a quantidade máxima de buracos não finalizados, começa em 100

void * joao(void * args) {
    while(1) {
        sem_wait(&MAXburacos);
        sem_wait(&pa);        
        printf("Joao cava buraco\n");
        sem_post(&pa);
        sem_post(&vazios);
        //sem_post(&buracos);
    }
}

void * pedro(void * args) {
    while(1) {
        sem_wait(&vazios);
        printf("Pedro semeia\n");
        sem_post(&afechar);
    }
}

void * paulo(void * args) {
    while(1) {
        //sem_wait(&buracos);
        
        sem_wait(&afechar);
        sem_wait(&pa);
        printf("Paulo fecha buraco\n");
        sem_post(&pa);
        sem_post(&MAXburacos);
    }
}

int main(int argc, char ** args) {

    //get args: size of buffer 1 and size of buffer 2
    if (argc != 2) {
        printf("Erro: argumentos inválidos. Informe o inteiro com a quantidade máxima de buracos\n");
        exit(1);
    }

    int MAX = atoi(args[1]);

    //test arguments
    if (MAX <= 0) {
        printf("Erro: argumentos inválidos. Informe o inteiro com a quantidade máxima de buracos\n");
        exit(1);
    }
    
    //init the semaphores
    /*sem_init(
    *   pointer to semaphore you want to init,
    *   0 if you want this semaphore to be ahared between threads,
    *   value you want to init semaphore with
    */
    int semReturnTest = sem_init(&vazios, 0, 0);
    semReturnTest = sem_init(&afechar, 0, 0);
    semReturnTest += sem_init(&pa, 0, 1);
    semReturnTest += sem_init(&MAXburacos, 0, MAX);
    
    //test if error ocourred with sem_init
    if (semReturnTest != 0) {
        lancaErro("Não foi possível iniciar semáforos", errno);
    }
    
    //start threads
    pthread_t threadJoao;
    pthread_t threaPedro;
    pthread_t threadPaulo;
    int errTest = pthread_create(&threadJoao, NULL, joao, NULL);
    errTest += pthread_create(&threaPedro, NULL, pedro, NULL);
    errTest += pthread_create(&threadPaulo, NULL, paulo, NULL);
    if (errTest != 0){
        lancaErro("Não foi possível criar threads", errno);
    }

    //join threads
    errTest = pthread_join(threadJoao, NULL);
    errTest += pthread_join(threaPedro, NULL);
    errTest += pthread_join(threadPaulo, NULL);
    if (errTest != 0){
        lancaErro("Não foi possível esperar threads criadas", errno);
    }
    return 0;
}