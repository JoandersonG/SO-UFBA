#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include "LancaErro.c"

typedef struct
{
  bool dorme; 
} Monitor;


#define MAXNUMESTUDANTES 100;

int statusErro = 0;

int numCadeiras = 0;
int numEstudantes = 0;

pthread_t estudantes[100];
Monitor monitor;

sem_t cadeiras;
int cadeirasOcupadas = 0;

pthread_mutex_t acessoCadeira;
pthread_mutex_t atendimento;

void *estudanteAtendido(void* i){
    int index = (int) i;
    
    statusErro = pthread_mutex_lock(&acessoCadeira);
    if(statusErro < 0) lancaErro("Problema para obter o mutex de acesso as cadeiras", errno);

    if(sem_trywait(&cadeiras) == -1 && errno == EAGAIN){
        printf("Estudante %d não foi atendido por falta de cadeiras.\n", index);
        statusErro = pthread_mutex_unlock(&acessoCadeira);
        if(statusErro < 0) lancaErro("Problema para liberar o mutex de acesso as cadeiras", errno);
        return;
    }
    cadeirasOcupadas++;
    statusErro = pthread_mutex_unlock(&acessoCadeira);
    if(statusErro < 0) lancaErro("Problema para liberar o mutex de acesso as cadeiras", errno);

    

    statusErro = pthread_mutex_lock(&atendimento);
    if(statusErro < 0) lancaErro("Problema para obter o mutex de atendimento", errno);
    if(monitor.dorme){
        printf("Estudante %d acorda monitor.\n", index);
        monitor.dorme = false;
    }
    printf("Estudante %d sendo atendido.\n", index);

    statusErro = pthread_mutex_lock(&acessoCadeira);
    if(statusErro < 0) lancaErro("Problema para obter o mutex de acesso as cadeiras", errno);
    statusErro = sem_post(&cadeiras);
    if(statusErro < 0) lancaErro("Problema para retirar item do semaforo de cadeiras", errno);
    cadeirasOcupadas--;

    printf("Estudante %d foi atendido.\n", index);

    if(cadeirasOcupadas == 0){
        printf("Monitor dormindo...\n");
        monitor.dorme = true;
    }

    statusErro = pthread_mutex_unlock(&acessoCadeira);
    if(statusErro < 0) lancaErro("Problema para liberar mutex de acesso a cadeiras", errno);
    statusErro = pthread_mutex_unlock(&atendimento);
    if(statusErro < 0) lancaErro("Problema para liberar o mutex de atendimento", errno);

    
    pthread_exit(index);
}

int main(int argc, char *argv[]){
    printf("Digite o numero maximo de cadeiras: ");
    scanf("%d", &numCadeiras);

    if(numCadeiras < 2){
        printf("É preciso ter pelo menos duas cadeiras.");
        return 0;
    }
    //Inicializando semáforo (cadeiras)
    sem_init(&cadeiras, 0, numCadeiras);

    //Inicializando muter (atendimento)
    statusErro = pthread_mutex_init(&atendimento, NULL);
    if(statusErro < 0) lancaErro("Problema para iniciar o mutex de atendimento", errno);
    statusErro = pthread_mutex_init(&acessoCadeira, NULL);
    if(statusErro < 0) lancaErro("Problema para iniciar o mutex de acesso as cadeiras", errno);

    
    while(1){
        cadeirasOcupadas = 0;
        printf("Digite o numero de estudantes para atendimento: ");
        scanf("%d", &numEstudantes);

        if(numEstudantes > 100){
            printf("Nao e possível ter mais de 100 estudantes.");
            return 0;
        }

        for(int index = 0; index < numEstudantes; index++){
            statusErro = pthread_create(&estudantes[index], NULL, estudanteAtendido, (void*)index);
            if(statusErro != 0){
                lancaErro("Problemas ao criar uma thread", errno);
            }
        }

        void* thread_res;
        for(int index = 0; index < numEstudantes; index++){
            statusErro = pthread_join(estudantes[index], NULL);
            if(statusErro != 0){
                lancaErro("Problemas ao esperar uma thread", errno);
            }
        }
    
    }
    sem_destroy(&cadeiras);

    return 0;
}