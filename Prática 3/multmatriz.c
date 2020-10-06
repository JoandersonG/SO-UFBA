#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include "LancaErro.c"

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

void * calculaEmThread(void * args) {


    //recupera argumentos
    int threadAtual = *((int *) args);

    //executa
    while (1) {

        //bloqueia essa área do código para múltiplas threads não realizarem a mesma operação
        pthread_mutex_lock(&mutex);
        if (taskNumber <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }
        taskNumber--;

        //recupera linha e coluna a serem multiplicadas
        int linhaA = tasks[taskNumber][0];
        int colunaB = tasks[taskNumber][1];

        //fijm do bloqueio da érea de código
        pthread_mutex_unlock(&mutex);

        printf("thread: %d. Executando a task %d\n",threadAtual, taskNumber);

        //executa operação e armazena na matriz final
        matrizProduto[linhaA][colunaB] = calculaValor(matrizA[linhaA],matrizB[colunaB]);
        printf("calculado a task %d: multiplicando linha %d por coluna %d e total: %f\n", taskNumber, linhaA, colunaB, matrizProduto[linhaA][colunaB]);

    }
    return NULL;
}

void freeMatrizA(int tam) {
    if (matrizA == NULL) return;
    for (int i = 0; i < tam; i++) free(matrizA[i]);
    free(matrizA);
}
void freeMatrizB(int tam) {
    if (matrizB == NULL) return;
    for (int i = 0; i < tam; i++) free(matrizB[i]);
    free(matrizB);
}
void freeMatrizProduto(int tam) {
    if (matrizProduto == NULL) return;
    for (int i = 0; i < tam; i++) free(matrizProduto[i]);
    free(matrizProduto);
}

int main(int argc, char ** argv) {

    //testes e mensagens de erro do argumetno fornecido
    if(argc < 6) {
        printf("Impossível iniciar o programa. Erro: argumentos insuficientes\n");
        printf("Forneça linha e coluna da primeira matriz, linha e coluna da segunda matriz e quantidade de threads, separados por um espaço em branco. \n");
        return 1;
    }

    //armazena os argumentos em variáveis identificáveis
    int linhasA = atoi(argv[1]); //armazena a quantidade de linhas da matriz A
    int colunasA = atoi(argv[2]); //quantidade de colunas na matriz A
    int linhasB = atoi(argv[3]); //quantidade de linhas da segunda matriz
    int colunasB = atoi(argv[4]); //quantidade de colunas da segunda matriz

    //teste de possibilidade de multiplicação de matriz
    if (colunasA != linhasB || linhasA <= 0 || linhasB <= 0 || colunasA <= 0 || colunasB <= 0) {
        printf("Impossível calcular. Erro: matrizes com valores inválidos de linha/coluna\n");
        return 1;
    }

    //define o máximo dee threads a serem usadas
    int QTD_THREADS = atoi(argv[5]);
    //testa validade de QTD_THREADS
    if (QTD_THREADS <= 0) {
        printf("Impossível calcular. Erro: quantidade de threads informada é inválida.\n");
        return 1;
    }
    pthread_t threads[QTD_THREADS];

    //cria as matrizes A e B
    matrizA = malloc(sizeof(double *) * linhasA);
    if (matrizA == NULL) {
        //não conseguiu alocar memória
        printf("Impossível executar programa. Erro: memória insuficiente.");
        return 1;
    }
    for (int i = 0; i < linhasA; ++i) {
        matrizA[i] = malloc(sizeof(double) * colunasA);
        if (matrizA[i] == NULL) {
        //não conseguiu alocar memória
        printf("Impossível executar programa. Erro: memória insuficiente.");
        freeMatrizA(linhasA);
        return 1;
    }
    }
    //invertendo as colunas e linhas de B para permitir calcular usando o vetor linha
    matrizB = malloc(sizeof(double *) * colunasB);
    if (matrizB == NULL) {
        //não conseguiu alocar memória
        printf("Impossível executar programa. Erro: memória insuficiente.");
        freeMatrizA(linhasA);
        return 1;
    }
    for (int i = 0; i < colunasB; ++i) {
        matrizB[i] = malloc(sizeof(double) * linhasB);
        if (matrizB[i] == NULL) {
        //não conseguiu alocar memória
        printf("Impossível executar programa. Erro: memória insuficiente.");
        freeMatrizA(linhasA);
        free(matrizB);
        return 1;
    }
    }

    //salvo globalmente o total de linhas de A para cálculos posteriores
    qtdLinhasA = linhasA;

    //cria uma matriz das dimensões que uma matriz produto precisa para o tamanho de A e B
    matrizProduto = malloc( sizeof(double*) * linhasA);
    if (matrizProduto == NULL) {
        //não conseguiu alocar memória
        printf("Impossível executar programa. Erro: memória insuficiente.");
        freeMatrizA(linhasA);
        freeMatrizB(colunasB);
        free(matrizProduto);
        return 1;
    }
    for (int i = 0; i < linhasA; ++i) {
        matrizProduto[i] = malloc( sizeof(double) * colunasB);
        if (matrizProduto[i] == NULL) {
            //não conseguiu alocar memória
            printf("Impossível executar programa. Erro: memória insuficiente.");
            freeMatrizA(linhasA);
            freeMatrizB(colunasB);
            free(matrizProduto);
            return 1;
        }
    }
    //todo: teste se alocações falharem

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

    //necessário para usar aleatório
    srand(time(0));;

    //popula matriz A
    for (int i = 0; i < linhasA; ++i) {
        for (int j = 0; j < colunasA; ++j) {
            matrizA[i][j] = rand() % 1000;
        }
    }

    //popula matriz B
    for (int i = 0; i < colunasB; ++i) {
        for (int j = 0; j < linhasB; ++j) {
            matrizB[i][j] = rand() % 1000;
        }
    }

    //aqui guardo as contas que precisarão ser feitas
    taskNumber = linhasA * colunasB;
    tasks = malloc(sizeof(int *) * taskNumber);
    if (tasks == NULL) {
        //não conseguiu alocar memória
        printf("Impossível executar programa. Erro: memória insuficiente.");
        freeMatrizA(linhasA);
        freeMatrizB(colunasB);
        freeMatrizProduto(linhasA);
        return 1;
    }
    for (int i = 0; i < taskNumber; ++i) {
        tasks[i] = malloc(sizeof(int) * 2);
        if (tasks[i] == NULL) {
        //não conseguiu alocar memória
            printf("Impossível executar programa. Erro: memória insuficiente.");
            freeMatrizA(linhasA);
            freeMatrizB(colunasB);
            freeMatrizProduto(linhasA);
            return 1;
        }
    }
    /*task agora é um do tipo:
     * [L1, C2]
     * [L1, C3]
     * [L1, C4]
     * ...
     * Com L1 sendo o índice da linha de A a ser multiplicada pela coluna com índice C2
     * e armazenada na matriz resultante em [L1][C2]
     */


    //calcula a matriz produto
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

    //inicia as threads
    int tid[QTD_THREADS];
    for (int i = 0; i < QTD_THREADS; i++) {
        tid[i] = i;
        int errTest = pthread_create(&(threads[i]), NULL, calculaEmThread, &(tid[i]));
        if (errTest != 0){
            lancaErro(errTest);
            //printf("Um erro ocorreu ao iniciar a thread %d. Erro: %s", i, strerror(errTest));
        }
        printf("iniciando a thread %d. Seu id é %lu \n", i, threads[i]);
    }

    //espera threads finalizarem
    for (int i = 0; i < QTD_THREADS; ++i) {
        int errorTest = pthread_join(threads[i], NULL);
        if (errorTest != 0) {
            lancaErro(errorTest);
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
