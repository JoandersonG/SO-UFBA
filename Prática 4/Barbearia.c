#include <stdio.h>
#include <semaphore.h>
#include <errno.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include "LancaErro.c"

sem_t entra_barbearia;//controla a quantidade de clientes da barbearia, inicia em 50
sem_t espaco_sofa;//controla a quantidade de espaços para cliente no sofá, inicia em 4
sem_t cliente_sofa;//controla a quantidade de clientes no sofá no momento, inicia em 0
sem_t atendimento_finalizado;//controla quando o atendimento é finalizado e o cliente sai da barbearia, inicia em 0;
sem_t barbeiro;//controla a quantidade de barbeiros livres para executar uma ação, inicia em 3
sem_t a_pagar;//controla a quantidade de clientes que finalizou o corte e precisa pagar, inicia em 0;

void down(sem_t * semaphore) {
    sem_wait(semaphore);
}

void up(sem_t * semaphore) {
    sem_post(semaphore);
}

void corta_o_cabelo() {
    //sleep(1);
    printf("Cortando cabelo de cliente\n");
}
void aceita_pagamento() {
    //sleep(1);
    printf("Pagamento Realizado\n");
}

void * Cliente(void * args) {
    printf("Cliente deseja entrar na barbearia\n");
    down(&entra_barbearia); //espera até poder entrar
    printf("Cliente entra na barbearia\n");
    down(&espaco_sofa);//espera até poder sentar
    printf("Cliente senta no sofá\n");
    up(&cliente_sofa);//há um cliente
    printf("Há um cliente no sofá\n");
    down(&atendimento_finalizado);//espera até ter cortado e pago
    printf("Cliente finalizou corte e pagamento\n");
    up(&entra_barbearia);//sai da barbearia
    printf("Cliente sai da barbearia\n");
}

void * Barbeiro(void * args) {
    while (1){
        down(&cliente_sofa);//dorme ate ter cliente para ser atendido
        printf("Barbeiro encontrou com cliente para corte\n");
         printf("Esperando barbeiro disponível para corte\n");
        down(&barbeiro);//para coordenar entre os papeis de Barbeiro e Caixa
        printf("Barbeiro disponível para corte\n");
        up(&espaco_sofa);//cliente saiu do sofá para a cadeira de barbeiro
        printf("Barbeiro faz com que cliente libere uma vaga no sofá\n");
        corta_o_cabelo();
        up(&a_pagar);//+1 cliente para pagar
        printf("Barbeiro coloca cliente na fila para pagar\n");
        up(&barbeiro);//barbeiro finalizou o corte
        printf("Barbeiro finalizou o corte\n");
    }
}

void * Caixa(void * args){
    while(1) {
        printf("Caixa esperando cliente para aceitar pagamento\n");
        down(&a_pagar);//espera ter cliente para pagar
        printf("Caixa esperando encontrou cliente para pagamento\n");
        down(&barbeiro);//requisita um barbeiro para executar
        printf("Caixa encontrou um barbeiro para realizar a transação\n");
        aceita_pagamento();
        printf("Caixa aceitou pagamento\n");
        up(&barbeiro);//libera barbeiro
        printf("Caixa libera barbeiro para ourtas funções\n");
        up(&atendimento_finalizado);
        printf("Caixa: atendimento de cliente finalizado\n");
    }
    
}

int main(int argc, char ** args) {

    //tests arguments
    if(argc != 2) {
        printf("Erro: argumentos inválidos. Informe a quantidade de clientes\n");
        exit(1);
    }
    //save arguments into variables
    int numClientes = atoi(args[1]);

    //test argument value
    if (numClientes <= 0) {
        printf("Erro: argumentos inválidos. Informe um inteiro maior que 0\n");
        exit(1);
    }

    //init the semaphores
    /*sem_init(
    *   pointer to semaphore you want to init,
    *   0 if you want this semaphore to be ahared between threads,
    *   value you want to init semaphore with
    */
    int semReturnTest = sem_init(&entra_barbearia, 0, 50);
    semReturnTest = sem_init(&espaco_sofa, 0, 4);
    semReturnTest += sem_init(&cliente_sofa, 0, 0);
    semReturnTest += sem_init(&atendimento_finalizado, 0, 0);
    semReturnTest += sem_init(&barbeiro, 0, 3);
    semReturnTest += sem_init(&a_pagar, 0, 0);
    
    //test if error ocourred with sem_init
    if (semReturnTest != 0) {
        lancaErro("Não foi possível iniciar semáforos", errno);
    }

    //start threads
    pthread_t threadsClientes[numClientes];
    pthread_t threadsBarbeiros[3];//3 threads barbeiros
    pthread_t threadCaixa;//1 caixa
    int errTest = pthread_create(&threadCaixa, NULL, Caixa, NULL);
    for (int i = 0; i < 3; i++) {
        errTest += pthread_create(&threadsBarbeiros[i], NULL, Barbeiro, NULL);
    }
    for (int i = 0; i < numClientes; i++) {
        errTest += pthread_create(&threadsClientes[i], NULL, Cliente, NULL);
    }

    //tests result from threads creation
    if (errTest != 0){
        lancaErro("Não foi possível criar threads", errno);
    }

    //join threads
    errTest = pthread_join(threadCaixa, NULL);
    for (int i = 0; i < 3; i++) {
        errTest += pthread_join(threadsBarbeiros[i], NULL);
    }
    for (int i = 0; i < numClientes; i++) {
        errTest += pthread_join(threadsClientes[i], NULL);
    }
    if (errTest != 0){
        lancaErro("Não foi possível esperar threads criadas", errno);
    }
    return 0;
}