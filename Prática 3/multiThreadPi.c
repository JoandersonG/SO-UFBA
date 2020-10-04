#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <pthread.h>
#include <errno.h>
#include "LancaErro.c"

const long int numTermos = 1000000000;

/* VARIÁVEL QUE GUARDA O NPUMERO DE THREADS */
int threadsNum = 32;
pthread_t threads[32];
double result[32] = {0};

void *calculadorPi(void *vars){
  int iteracoesNum = (int)(numTermos / threadsNum);
  int index = (int) vars;
  int threadNum = index;

  for(; index < iteracoesNum; index += threadsNum){
    double aux = (pow(-1, index))/(2*index + 1);
    result[threadNum] += (pow(-1, index))/(2*index + 1);
  }

  pthread_exit(vars);
}

int main(){
  void* thread_res;
  int status;

  for(int i = 0; i < threadsNum; i++){
    status = pthread_create (&threads[i], NULL, calculadorPi, (void*)i);
    if(status != 0){
      lancaErro(errno);
    }
  }

  for(int i = 0; i < threadsNum; i++){
    status = pthread_join(threads[i], &thread_res);
    if(status != 0){
      lancaErro(errno);
    }
  }

  double finalResult = 0;
  for(int i = 0; i < threadsNum; i++){
    finalResult += result[i];
  }

  printf("Encerrando threads\n");
  printf("Resultado final: %f\n", finalResult);

  return 0;
}
