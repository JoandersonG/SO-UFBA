#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <errno.h>
#include <string.h>

int** tasks;
double** matrizProduto;
double** matrizA;
double** matrizB;
int qtdLinhasA = 0;
int taskNumber = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


double calculaValor(double* linha, double* coluna) {
    //linha e coluna possuem mesmo tamanho, já que é uma multiplicação de matrizes válida

    double total = 0;
    for (int i = 0; i < qtdLinhasA; ++i) {
        total += linha[i] * coluna[i];
    }

    return total;
}

int getTaskLinhaA() {
    //bloquear acesso por mais de uma thread. Isso funciona?
    pthread_mutex_lock(&mutex);
    int tn = tasks[taskNumber-1][0];
    pthread_mutex_unlock(&mutex);
    return tn;
}
int getTaskColunaB() {
    //bloquear acesso por mais de uma thread. Isso funciona?
    pthread_mutex_lock(&mutex);
    int tn = tasks[taskNumber-1][1];
    pthread_mutex_unlock(&mutex);
    return tn;
}
void * calculaEmThread(void * args) {


    //recupero argumentos
    int threadAtual = *((int *) args); //não funciona apropriadamente

    //executa
    while (taskNumber > 0) {
        printf("thread: %d. Executando a task %d\n",threadAtual, taskNumber-1);

        //recupera tarefa
        int linhaA = getTaskLinhaA();
        int colunaB = getTaskColunaB();
        matrizProduto[linhaA][colunaB] = calculaValor(matrizA[linhaA],matrizB[colunaB]);
        printf("calculado a task %d: multiplicando linha %d por coluna %d e total: %f\n", taskNumber - 1, linhaA, colunaB, matrizProduto[linhaA][colunaB]);
        taskNumber--;
    }

    //salva valor
}

int main(int argc, char ** argv) {

    //testar se os argumentos são válidos (quantidade correta e ints válidos e se dá para fazer multiplicação [tam linha x colunas])

    //armazena os argumentos em variáveis identificáveis
    int linhasA = atoi(argv[1]); //armazena a quantidade de linhas da matriz A
    int colunasA = atoi(argv[2]); //quantidade de colunas na matriz A
    int linhasB = atoi(argv[3]); //quantidade de linhas da segunda matriz
    int colunasB = atoi(argv[4]); //quantidade de colunas da segunda matriz

    //define o máximo dee threads a serem usadas
    int QTD_THREADS = atoi(argv[5]);
    pthread_t threads[QTD_THREADS];
    printf("qtd_threads informada: %d\n", QTD_THREADS);

    //cria as matrizes A e B
    matrizA = malloc(sizeof(double *) * linhasA);
    for (int i = 0; i < linhasA; ++i) {
        matrizA[i] = malloc(sizeof(double) * colunasA);
    }
    //invertendo as colunas e linhas para permitir calcular usando linhas.
    matrizB = malloc(sizeof(double *) * colunasB);
    for (int i = 0; i < colunasB; ++i) {
        matrizB[i] = malloc(sizeof(double) * linhasB);
    }

    //salvo globalmente o total de linhas de A para cálculos posteriores
    qtdLinhasA = linhasA;


    //cria uma matriz das dimensões que uma matriz produto precisa para o tamanho de A e B
    matrizProduto = malloc( sizeof(double*) * linhasA);
    for (int i = 0; i < linhasA; ++i) {
        matrizProduto[i] = malloc( sizeof(double) * colunasB);
    }
    //todo: teste se alocação falhar

    //zerar as matrizes. preciso?
    for (int i = 0; i < linhasA; ++i) {
        for (int j = 0; j < colunasB; ++j) {
            matrizProduto[i][j] = 0;
        }

        for (int j = 0; j < colunasA; ++j) {
            matrizA[i][j] = 0;
        }
    }
    for (int i = 0; i < linhasB; ++i) {
        for (int j = 0; j < colunasB; ++j) {
            matrizB[j][i] = 0;
        }
    }
    //printf("aqui2");
    //necessário para usar aleatório. Temporariamente excluído para testar corretude
    //todo: srand(time(0));;

    //popula matriz A
    for (int i = 0; i < linhasA; ++i) {
        for (int j = 0; j < colunasA; ++j) {
            matrizA[i][j] = rand() % 5;
        }
    }

    //popula matriz B
    for (int i = 0; i < colunasB; ++i) {
        for (int j = 0; j < linhasB; ++j) {
            matrizB[i][j] = rand() % 5;
        }
    }

    //aqui guardo as contas que precisarão ser feitas
    taskNumber = linhasA * colunasB;
    tasks = malloc(sizeof(int *) * taskNumber);
    for (int i = 0; i < taskNumber; ++i) {
        tasks[i] = malloc(sizeof(int) * 2);
    }
    /*task agora é um do tipo:
     * [L1, C2]
     * [L1, C3]
     * [L1, C4]
     * ...
     * Com L1 sendo a linha de A a ser multiplicada pela coluna C2 e armazenada na matriz resultante em [1][2]
     */

    
    //calcula a matriz produto
    int qtdThreadsUsadas = 0;
    int posicaoTask = 0;
    for (int i = 0; i < linhasA; ++i) {
        for (int j = 0; j < colunasB; ++j) {

            //salvo as tarefas que deverão ser executadas pelas threads
            tasks[posicaoTask][0] = i; //multiplicar a linha i de A
            tasks[posicaoTask][1] = j; //pela coluna j de B
            posicaoTask++;
        }
    }

    printf("Tarefas a serem cumpridas:\n");
    for (int i = 0; i < linhasA * colunasB; ++i) {
        printf("tarefa %d: multiplicar linha %d por coluna %d\n ",i, tasks[i][0], tasks[i][1]);
    }

    //iniciar as threads
    int tid[QTD_THREADS];
    for (int i = 0; i < QTD_THREADS; i++) {
        tid[i] = i;
        int errTest = pthread_create(&(threads[i]), NULL, calculaEmThread, &(tid[i]));
        if (errTest != 0){
            printf("Um erro ocorreu ao iniciar a thread %d. Erro: %s", i, strerror(errTest));
        }
        printf("iniciando a thread %d. Seu id é %lu \n", i, threads[i]);
    }

    //esperar threads finalizarem
    for (int i = 0; i < QTD_THREADS; ++i) {
        int errorTest = pthread_join(threads[i], NULL);
        //printf("error test: %d\n", errorTest);
        if (errorTest != 0) {
            printf("Um erro ocorreu ao finalizar thread %d. Erro: %s\n", i, strerror(errorTest));
        }

    }


    printf("Matriz A:\n");
    for (int i = 0; i < linhasA; ++i) {
        for (int j = 0; j < colunasA; ++j) {
            printf("%f ",matrizA[i][j]);
        }
        printf("\n");
    }

    printf("\nMatriz B:\n");
    for (int i = 0; i < linhasB; ++i) {
        for (int j = 0; j < colunasB; ++j) {
            printf("%f ",matrizB[j][i]);
        }
        printf("\n");
    }

    printf("\nMatriz Produto:\n");
    for (int i = 0; i < linhasA; ++i) {
        for (int j = 0; j < colunasB; ++j) {
            printf("%f ",matrizProduto[i][j]);
        }
        printf("\n");
    }
    return 0;
}
