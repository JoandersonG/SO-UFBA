#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int media, maior, menor;
int qtd_num = 0;

//Função que calcula a Media
void* calculaMedia(void* arg)
{	
	int* l = (int*) arg;
	int soma = 0;
	
	for(int i = 0; i < qtd_num; i++){
		soma += l[i];
	}
	
	media = soma/qtd_num;
	pthread_exit(0);
}

//Função que calcula o maior numero
void* calcula_maior(void* arg)
{
	int* l = (int*) arg;
	maior = 0;
	
	for(int i = 0; i < qtd_num; i++){
		if ( l[i] > maior )
			maior = l[i];
	}
	
	pthread_exit(0);
}

//Função que calcula o menor numero
void* calcula_menor(void* arg)
{
	int* l = (int*) arg;
	menor = 10000000;
	
	for(int i = 0; i < qtd_num; i++){
		if ( l[i] < menor )
			menor = l[i];
	}
	
	pthread_exit(0);
}


int main(int argc, char *argv[ ] ){
	
	
	qtd_num = argc - 1;
	int lista[qtd_num];
	
  	for( int cont=1; cont < argc; cont++)
	{    
	    lista[cont-1] = atoi(argv[cont]);
  	}
	
	//thread ID
	pthread_t tid, tid2, tid3;
	
	
	media = pthread_create(&tid, NULL, calculaMedia, lista);
	pthread_join(tid, NULL);
	maior = pthread_create(&tid2, NULL, calcula_maior, lista);
	pthread_join(tid2, NULL);
	menor = pthread_create(&tid3, NULL, calcula_menor, lista);
	pthread_join(tid3, NULL);
	printf("%d %d %d\n", media, maior, menor);
  	
  	
	  return 0;
}
